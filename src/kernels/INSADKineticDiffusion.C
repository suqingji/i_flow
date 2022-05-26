#include "INSADKineticDiffusion.h"

registerMooseObject("iFlowApp", INSADKineticDiffusion);

InputParameters
INSADKineticDiffusion::validParams()
{
  auto params = ADDiffusion::validParams();
  params.addClassDescription("Same as `Diffusion` in terms of physics/residual, but the Jacobian "
                             "is computed using forward automatic differentiation");
  return params;
}

INSADKineticDiffusion::INSADKineticDiffusion(const InputParameters &parameters) : ADDiffusion(parameters),
                                                                                  _mu_k(getMaterialProperty<Real>("mu_k"))

{
}

ADRealVectorValue
INSADKineticDiffusion::precomputeQpResidual()
{
  return _mu_k[_qp] * ADDiffusion::precomputeQpResidual();
}
