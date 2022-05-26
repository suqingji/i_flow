#include "INSADKOTurbStabilized.h"

registerMooseObject("iFlowApp", INSADKOTurbStabilized);

InputParameters
INSADKOTurbStabilized::validParams()
{
  InputParameters params = INSADTauMaterialTempl<INSADKOTurb>::validParams();
  params.addClassDescription("This is the material class used to compute the stabilization "
                             "parameter tau for momentum and tau_energy for the energy equation.");
  return params;
}

INSADKOTurbStabilized::INSADKOTurbStabilized(const InputParameters &parameters)
    : INSADTauMaterialTempl<INSADKOTurb>(parameters),
      _second_kinetic(adCoupledSecond("kinetic")),
      _second_omega(adCoupledSecond("omega")),
      _tau_kinetic(declareADProperty<Real>("tau_kinetic")),
      _tau_omega(declareADProperty<Real>("tau_omega")),
      _kinetic_strong_residual(declareADProperty<Real>("kinetic_strong_residual")),
      _omega_strong_residual(declareADProperty<Real>("omega_strong_residual"))
{
}

void INSADKOTurbStabilized::computeQpProperties()
{
  INSADTauMaterialTempl<INSADKOTurb>::computeQpProperties();

  auto dissipation_coefficient_kinetic = _mu_k[_qp] / _rho[_qp].value();
  auto dissipation_coefficient_omega = _mu_omega[_qp] / _rho[_qp].value();
  auto transient_part_kinetic = _has_transient ? 4. / (_dt * _dt) : 0.;
  auto transient_part_omega = _has_transient ? 4. / (_dt * _dt) : 0.;
  _tau_kinetic[_qp] = _alpha / std::sqrt(transient_part_kinetic +
                                         (2. * _velocity[_qp].norm() / _hmax) *
                                             (2. * _velocity[_qp].norm() / _hmax) +
                                         9. * (4. * dissipation_coefficient_kinetic / (_hmax * _hmax)) *
                                             (4. * dissipation_coefficient_kinetic / (_hmax * _hmax)));
  _tau_omega[_qp] = _alpha / std::sqrt(transient_part_omega +
                                       (2. * _velocity[_qp].norm() / _hmax) *
                                           (2. * _velocity[_qp].norm() / _hmax) +
                                       9. * (4. * dissipation_coefficient_omega / (_hmax * _hmax)) *
                                           (4. * dissipation_coefficient_omega / (_hmax * _hmax)));

  // // Start with the conductive term
  _kinetic_strong_residual[_qp] = -_mu_k[_qp] * _second_kinetic[_qp].tr();
  _omega_strong_residual[_qp] = -_mu_omega[_qp] * _second_omega[_qp].tr();

  // // advective
  _kinetic_strong_residual[_qp] += _kinetic_advective_strong_residual[_qp];
  _omega_strong_residual[_qp] += _omega_advective_strong_residual[_qp];
  // // source
  _kinetic_strong_residual[_qp] += _kinetic_source_strong_residual[_qp];
  _omega_strong_residual[_qp] += _omega_source_strong_residual[_qp];
  if (_has_transient)
  {
    _kinetic_strong_residual[_qp] += _kinetic_td_strong_residual[_qp];
    _omega_strong_residual[_qp] += _omega_td_strong_residual[_qp];
  }

  // if (_has_transient)
  //   _kinetic_strong_residual[_qp] += _kinetic_td_strong_residual[_qp];
  // if (_has_transient)
  //   _omega_strong_residual[_qp] += _omega_td_strong_residual[_qp];
}