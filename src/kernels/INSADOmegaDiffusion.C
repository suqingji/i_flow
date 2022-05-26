#include "INSADOmegaDiffusion.h"

registerMooseObject("iFlowApp", INSADOmegaDiffusion);

InputParameters
INSADOmegaDiffusion::validParams()
{
  auto params = ADDiffusion::validParams();
  params.addClassDescription("Same as `Diffusion` in terms of physics/residual, but the Jacobian "
                             "is computed using forward automatic differentiation");
  return params;
}

INSADOmegaDiffusion::INSADOmegaDiffusion(const InputParameters &parameters) : ADDiffusion(parameters),
                                                                              _mu_omega(getMaterialProperty<Real>("mu_omega"))

{
}

ADRealVectorValue
INSADOmegaDiffusion::precomputeQpResidual()
{
  return _mu_omega[_qp] * ADDiffusion::precomputeQpResidual();
}
