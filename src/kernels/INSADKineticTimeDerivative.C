#include "INSADKineticTimeDerivative.h"

registerMooseObject("iFlowApp", INSADKineticTimeDerivative);

InputParameters
INSADKineticTimeDerivative::validParams()
{
  InputParameters params = ADTimeDerivative::validParams();
  params.addClassDescription(
      "AD Time derivative term $\\rho c_p \\frac{\\partial T}{\\partial t}$ of "
      "the heat equation for quasi-constant specific heat $c_p$ and the density $\\rho$.");
  return params;
}

INSADKineticTimeDerivative::INSADKineticTimeDerivative(
    const InputParameters & parameters)
  : ADTimeDerivative(parameters),
    _kinetic_td_strong_residual(getADMaterialProperty<Real>("kinetic_td_strong_residual"))
{
}

ADReal
INSADKineticTimeDerivative::precomputeQpResidual()
{
  return _kinetic_td_strong_residual[_qp];
}