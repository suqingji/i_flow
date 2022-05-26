#include "INSADKineticAdvection.h"

registerMooseObject("iFlowApp", INSADKineticAdvection);

InputParameters
INSADKineticAdvection::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription("This class computes the residual and Jacobian contributions for "
                             "temperature advection for a divergence free velocity field.");
  return params;
}

INSADKineticAdvection::INSADKineticAdvection(const InputParameters &parameters)
    : ADKernelValue(parameters),
      _kinetic_advective_strong_residual(
          getADMaterialProperty<Real>("kinetic_advective_strong_residual")),
      _kinetic_source_residual(
          getADMaterialProperty<Real>("kinetic_source_strong_residual"))
{
}

ADReal
INSADKineticAdvection::precomputeQpResidual()
{
  return _kinetic_advective_strong_residual[_qp] + _kinetic_source_residual[_qp];
}
