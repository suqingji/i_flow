#include "INSADOmegaAdvection.h"

registerMooseObject("iFlowApp", INSADOmegaAdvection);

InputParameters
INSADOmegaAdvection::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription("This class computes the residual and Jacobian contributions for "
                             "temperature advection for a divergence free velocity field.");
  return params;
}

INSADOmegaAdvection::INSADOmegaAdvection(const InputParameters &parameters)
    : ADKernelValue(parameters),
      _omega_advective_strong_residual(
          getADMaterialProperty<Real>("omega_advective_strong_residual")),
      _omega_source_residual(
          getADMaterialProperty<Real>("omega_source_strong_residual"))
{
}

ADReal
INSADOmegaAdvection::precomputeQpResidual()
{
  return _omega_advective_strong_residual[_qp] + _omega_source_residual[_qp];
}
