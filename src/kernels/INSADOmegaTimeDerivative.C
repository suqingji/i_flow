#include "INSADOmegaTimeDerivative.h"

registerMooseObject("iFlowApp", INSADOmegaTimeDerivative);

InputParameters
INSADOmegaTimeDerivative::validParams()
{
  InputParameters params = ADTimeDerivative::validParams();
  params.addClassDescription(
      "AD Time derivative term $\\rho c_p \\frac{\\partial T}{\\partial t}$ of "
      "the heat equation for quasi-constant specific heat $c_p$ and the density $\\rho$.");
  return params;
}

INSADOmegaTimeDerivative::INSADOmegaTimeDerivative(
    const InputParameters & parameters)
  : ADTimeDerivative(parameters),
    _omega_td_strong_residual(getADMaterialProperty<Real>("omega_td_strong_residual"))
{
}

ADReal
INSADOmegaTimeDerivative::precomputeQpResidual()
{
  return _omega_td_strong_residual[_qp];
}