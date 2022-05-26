#pragma once

#include "ADTimeDerivative.h"

class INSADOmegaTimeDerivative : public ADTimeDerivative
{
public:
  static InputParameters validParams();

  INSADOmegaTimeDerivative(const InputParameters & parameters);

protected:
  ADReal precomputeQpResidual() override;

  const ADMaterialProperty<Real> & _omega_td_strong_residual;
};