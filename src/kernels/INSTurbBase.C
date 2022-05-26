#include "INSTurbBase.h"
#include "Function.h"

InputParameters
INSTurbBase::validParams()
{
    InputParameters params = INSBase::validParams();
    params.addCoupledVar("kinetic", "kinetic");
    params.addCoupledVar("omega", "omega");
    params.addParam<int>("model", 4, "湍流模型");
    params.addParam<bool>(
        "supg", false, "Whether to perform SUPG stabilization of the momentum residuals");
    return params;
}

INSTurbBase::INSTurbBase(const InputParameters &parameters)
    : INSBase(parameters),
      _model(getParam<int>("model")),

      _w_var_number(coupled("omega")),

      // k-w turbulence model require variables of kinetic and omega
      // Coupled variables
      _kinetic(coupledValue("kinetic")),
      _omega(coupledValue("omega")),
      // Gradients
      _grad_kinetic(coupledGradient("kinetic")),
      _grad_omega(coupledGradient("omega")),
      // second derivative tensors
      _second_kinetic(coupledSecond("kinetic")),
      _second_omega(coupledSecond("omega")),
      // time derivatives
      _kinetic_dot(_is_transient ? coupledDot("kinetic") : _zero),
      _omega_dot(_is_transient ? coupledDot("omega") : _zero),
      _supg(getParam<bool>("supg")),
      // derivatives of time derivatives
      _d_kinetic_dot_du(_is_transient ? coupledDotDu("kinetic") : _zero),
      _d_omega_dot_du(_is_transient ? coupledDotDu("omega") : _zero),

      _mu_turb(getMaterialProperty<Real>("mu_turb")),
      _mu_l(getMaterialProperty<Real>("mu_l"))
{
    if (_supg && !_convective_term)
        mooseError(
            "It doesn't make sense to conduct SUPG stabilization without a convective term.湍流方程的");
}

Real INSTurbBase::computeQpResidual()
{
    RealVectorValue U(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
    Real r = 0;
    // viscous term
    r += computeQpResidualViscousPart();
    // convective term
    r += _rho[_qp] * (U * _grad_u[_qp]) * _test[_i][_qp];
    if (_supg)
        r += computeQpPGResidual();
    return r;
}

Real INSTurbBase::computeQpPGResidual()
{
    Real r = computeQpPGResidualallPart();
    return r;
}

Real INSTurbBase::computeQpJacobian()
{
    RealVectorValue U(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
    Real jac = 0;
    // viscous term
    jac += computeQpJacobianViscousPart();
    // convective term
    jac += _rho[_qp] * (U * _grad_phi[_j][_qp]) * _test[_i][_qp];
    if (_supg)
        jac += computeQpPGJacobian();
    return jac;
}

Real INSTurbBase::computeQpPGJacobian()
{
    Real jac = computeQpPGJacobianallPart();
    return jac;
}

Real INSTurbBase::computeQpOffDiagJacobian(unsigned jvar)
{
    if (jvar == _u_vel_var_number)
    {
        Real convective_part = _rho[_qp] * _phi[_j][_qp] * _grad_u[_qp](0) * _test[_i][_qp];
        return convective_part;
    }

    else if (jvar == _v_vel_var_number)
    {
        Real convective_part = _rho[_qp] * _phi[_j][_qp] * _grad_u[_qp](1) * _test[_i][_qp];
        return convective_part;
    }

    else if (jvar == _w_vel_var_number)
    {
        Real convective_part = _rho[_qp] * _phi[_j][_qp] * _grad_u[_qp](2) * _test[_i][_qp];
        return convective_part;
    }

    else if (jvar == _w_var_number)
    {
        Real jac = 0.09 * _rho[_qp] * _kinetic[_qp] * _phi[_j][_qp] * _test[_i][_qp];
        return jac;
    }
    else
        return 0;
}

Real INSTurbBase::Prod()
{
    Real Sij = (_grad_u_vel[_qp](0) + _grad_u_vel[_qp](0)) * _grad_u_vel[_qp](0) +
               (_grad_u_vel[_qp](1) + _grad_v_vel[_qp](0)) * _grad_u_vel[_qp](1) +
               (_grad_u_vel[_qp](2) + _grad_w_vel[_qp](0)) * _grad_u_vel[_qp](2) +
               (_grad_v_vel[_qp](0) + _grad_u_vel[_qp](1)) * _grad_v_vel[_qp](0) +
               (_grad_v_vel[_qp](1) + _grad_v_vel[_qp](1)) * _grad_v_vel[_qp](1) +
               (_grad_v_vel[_qp](2) + _grad_w_vel[_qp](1)) * _grad_v_vel[_qp](2) +
               (_grad_w_vel[_qp](0) + _grad_u_vel[_qp](2)) * _grad_w_vel[_qp](0) +
               (_grad_w_vel[_qp](1) + _grad_v_vel[_qp](2)) * _grad_w_vel[_qp](1) +
               (_grad_w_vel[_qp](2) + _grad_w_vel[_qp](2)) * _grad_w_vel[_qp](2);
    return _mu_turb[_qp] * Sij;
}