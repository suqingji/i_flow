#include "INSADKOTurb.h"
#include "INSADObjectTracker.h"
#include "Function.h"

registerMooseObject("iFlowApp", INSADKOTurb);

InputParameters
INSADKOTurb::validParams()
{
    InputParameters params = INSADMaterial::validParams();
    params.addClassDescription("This material computes properties needed for stabilized formulations "
                               "of the mass, momentum, and energy equations.");
    params.addRequiredCoupledVar("kinetic", "The kinetic");
    params.addRequiredCoupledVar("omega", "The omega");
    return params;
}

INSADKOTurb::INSADKOTurb(const InputParameters &parameters)
    : INSADMaterial(parameters),
      _kinetic(adCoupledValue("kinetic")),
      _grad_kinetic(adCoupledGradient("kinetic")),
      _kinetic_advective_strong_residual(declareADProperty<Real>("kinetic_advective_strong_residual")),
      _kinetic_td_strong_residual(declareADProperty<Real>("kinetic_td_strong_residual")),
      _kinetic_source_strong_residual(declareADProperty<Real>("kinetic_source_strong_residual")),
      _mu_k(declareProperty<Real>("mu_k")),

      _omega(adCoupledValue("omega")),
      _grad_omega(adCoupledGradient("omega")),
      _omega_advective_strong_residual(declareADProperty<Real>("omega_advective_strong_residual")),
      _omega_td_strong_residual(declareADProperty<Real>("omega_td_strong_residual")),
      _omega_source_strong_residual(declareADProperty<Real>("omega_source_strong_residual")),
      _mu_omega(declareProperty<Real>("mu_omega")),
      _mu_turb(declareADProperty<Real>("mu_turb")),
      _mu_l(getADMaterialProperty<Real>("mu_l")),
      _mu_eff(declareADProperty<Real>("mu"))
{
}

void INSADKOTurb::subdomainSetup()
{
    INSADMaterial::subdomainSetup();
    if (_has_transient)
    {
        _kinetic_dot = &adCoupledDot("kinetic");
        _omega_dot = &adCoupledDot("omega");
    }
}

void INSADKOTurb::computeQpProperties()
{
    INSADMaterial::computeQpProperties();
    // stress
    ADRealTensorValue stress = _grad_velocity[_qp] + _grad_velocity[_qp].transpose();
    ADRealTensorValue Ome = _grad_velocity[_qp] - _grad_velocity[_qp].transpose();
    ADReal OOS = 0;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                 OOS += 0.25 * Ome(i, j) * Ome(j, k) * stress(k, i);
            }
        }
    }

    ADReal Xomega = abs(OOS / (0.09 * _omega[_qp]) / (0.09 * _omega[_qp]) / (0.09 * _omega[_qp]));
    ADReal Xkinetic = 1. / _omega[_qp] / _omega[_qp] / _omega[_qp] * (_grad_kinetic[_qp](0) * _grad_omega[_qp](0) + _grad_kinetic[_qp](1) * _grad_omega[_qp](1) + _grad_kinetic[_qp](2) * _grad_omega[_qp](2));

    ADReal fbeta = (1 + 70 * Xomega) / (1 + 80 * Xomega);

    ADReal fbeta_star = 1;
    if (Xkinetic > 0)
    {
        fbeta_star = (1 + 680 * Xkinetic * Xkinetic) / (1 + 400 * Xkinetic * Xkinetic);
    }

    ADReal beta_star = 0.09 * fbeta_star;
    ADReal beta = 0.072 * fbeta;
    ADReal gamma = 0.8 - 0.28017;
    // _mu_turb[_qp] = std::max(0.0, _rho[_qp].value() * _kinetic[_qp].value() / _omega[_qp].value());
    _mu_turb[_qp] = std::max(0.0, _rho[_qp] * _kinetic[_qp] / _omega[_qp]);
    _mu_eff[_qp] = _mu_l[_qp] + _mu_turb[_qp].value();
    _mu_k[_qp] = _mu_l[_qp].value() + 0.5 * _mu_turb[_qp].value();
    _mu_omega[_qp] = _mu_l[_qp].value() + 0.5 * _mu_turb[_qp].value();

    // 速度梯度和应力张量缩并,
    // Real Prod_kinetic = _mu_turb[_qp] * tensorReduce(_grad_velocity[_qp], stress).value();
    // Real Prod_omega = 5. / 9. * _rho[_qp].value() * tensorReduce(_grad_velocity[_qp], stress).value();

    ADReal Prod_kinetic = _mu_turb[_qp].value() * tensorReduce(_grad_velocity[_qp], stress);
    ADReal Prod_omega = gamma.value() * _rho[_qp] * tensorReduce(_grad_velocity[_qp], stress);

    _kinetic_advective_strong_residual[_qp] = _rho[_qp] * _velocity[_qp] * _grad_kinetic[_qp];
    _omega_advective_strong_residual[_qp] = _rho[_qp] * _velocity[_qp] * _grad_omega[_qp];

    // _kinetic_source_strong_residual[_qp] = 0.09 * _rho[_qp] * _omega[_qp] * _kinetic[_qp]; // - Prod_kinetic;
    // _omega_source_strong_residual[_qp] = 0.075 * _rho[_qp] * _omega[_qp] * _omega[_qp];    // - Prod_omega;
    _kinetic_source_strong_residual[_qp] = beta_star * _rho[_qp] * _omega[_qp] * _kinetic[_qp] - Prod_kinetic;
    _omega_source_strong_residual[_qp] = beta * _rho[_qp] * _omega[_qp] * _omega[_qp] - Prod_omega;

    if (_has_transient)
    {
        mooseAssert(_kinetic_dot, "The kinetic time derivative is null");
        mooseAssert(_omega_dot, "The omega time derivative is null");

        _kinetic_td_strong_residual[_qp] = _rho[_qp] * (*_kinetic_dot)[_qp];
        _omega_td_strong_residual[_qp] = _rho[_qp] * (*_omega_dot)[_qp];
    }
}
