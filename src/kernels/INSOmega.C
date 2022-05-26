#include "INSOmega.h"

registerMooseObject("iFlowApp", INSOmega);

InputParameters
INSOmega::validParams()
{
    InputParameters params = INSTurbBase::validParams();
    params.addClassDescription("This class computes momentum equation residual and Jacobian viscous "
                               "contributions for the 'Laplacian' form of the governing equations.");
    return params;
}

INSOmega::INSOmega(const InputParameters &parameters)
    : INSTurbBase(parameters),
      _u_dot(_var.uDot()),
      _du_dot_du(_var.duDotDu()),
      _second_u(_is_implicit ? _var.secondSln() : _var.secondSlnOld())
{
}

Real INSOmega::computeQpResidualViscousPart()
{
    Real r = 0.;
    if (_model == 4)
    {
        //粘性项
        r += (_mu_l[_qp] + 0.5 * _mu_turb[_qp]) * (_grad_u[_qp] * _grad_test[_i][_qp]);
        //生成项
        r -= (5. / 9.) * _omega[_qp] / _kinetic[_qp] * Prod() * _test[_i][_qp];
        //耗散项
        r += 0.075 * _rho[_qp] * _u[_qp] * _u[_qp] * _test[_i][_qp];
    }
    return r;
}

Real INSOmega::computeQpPGResidualallPart()
{
    Real r = 0.;
    RealVectorValue U(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
    //非稳态项
    r += _rho[_qp] * _u_dot[_qp];
    //对流项
    r += U * _grad_u[_qp];
    if (_model == 4)
    {
        //粘性项
        r -= (_mu_l[_qp] + 0.5 * _mu_turb[_qp]) * _second_u[_qp].tr();
        //生成项
        r -= (5. / 9.) * _omega[_qp] / _kinetic[_qp] * Prod();
        //耗散项
        r += 0.075 * _rho[_qp] * _u[_qp] * _u[_qp];
    }
    return tau_omega() * (U * _grad_test[_i][_qp]) * r;
}

Real INSOmega::computeQpJacobianViscousPart()
{
    Real jac = 0.;
    if (_model == 4)
    {
        //粘性项
        jac += (_mu[_qp] + 0.5 * _mu_turb[_qp]) * (_grad_phi[_j][_qp] * _grad_test[_i][_qp]);
        /* jac -= (_kinetic[_qp] * _phi[_j][_qp] / _omega[_qp] / _omega[_qp]) *
               (_grad_u[_qp] * _grad_test[_i][_qp]); */
        //生成项
        //难以构造jacobian
        //耗散项
        jac += 2. * 0.075 * _rho[_qp] * _u[_qp] * _phi[_j][_qp] * _test[_i][_qp];
    }
    return jac;
}

Real INSOmega::computeQpPGJacobianallPart()
{
    Real jac = 0.;
    RealVectorValue U(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
    //非稳态项d_transient_term_d_omega
    jac += _rho[_qp] * _phi[_j][_qp] * _du_dot_du[_qp];
    //对流项d_convective_term_d_omega
    jac += _rho[_qp] * U * _grad_phi[_j][_qp];
    if (_model == 4)
    {
        //粘性项d_viscous_term_d_omega
        jac -= (_mu_l[_qp] + 0.5 * _mu_turb[_qp]) * _second_phi[_j][_qp].tr();
        // jac -= (_kinetic[_qp] * _phi[_j][_qp] / _omega[_qp] / _omega[_qp]) * _second_u[_qp].tr();
        //生成项
        //无jacobian
        //耗散项
        jac += 2. * 0.075 * _rho[_qp] * _u[_qp] * _phi[_j][_qp];
    }
    return tau_omega() * (U * _grad_test[_i][_qp]) * jac;
}

Real INSOmega::computeQpOffDiagJacobianViscousPart(unsigned /*jvar*/)
{
    return 0.;
}

Real INSOmega::tau_omega()
{
    RealVectorValue U(_u_vel[_qp], _v_vel[_qp], _w_vel[_qp]);
    Real h = _current_elem->hmax();
    Real transient_part = _transient_term ? 4. / (_dt * _dt) : 0.;
    Real all_part = 0.;
    all_part += transient_part + (2. * U.norm() / h) * (2. * U.norm() / h);

    if (_model == 4)
    {
        Real nu = (_mu_l[_qp] + _mu_turb[_qp]) / _rho[_qp];
        all_part += 9. * (4. * nu / (h * h)) * (4. * nu / (h * h));
    }
    return _alpha / std::sqrt(all_part);
}