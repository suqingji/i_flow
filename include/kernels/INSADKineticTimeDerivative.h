#pragma once

#include "ADTimeDerivative.h"

class INSADKineticTimeDerivative : public ADTimeDerivative
{
public:
  static InputParameters validParams();

  INSADKineticTimeDerivative(const InputParameters & parameters);

protected:
  ADReal precomputeQpResidual() override;

  const ADMaterialProperty<Real> & _kinetic_td_strong_residual;
};