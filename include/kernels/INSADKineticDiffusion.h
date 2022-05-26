#pragma once

#include "ADDiffusion.h"

class INSADKineticDiffusion : public ADDiffusion
{
public:
  static InputParameters validParams();

  INSADKineticDiffusion(const InputParameters &parameters);

protected:
  virtual ADRealVectorValue precomputeQpResidual() override;

  const MaterialProperty<Real> &_mu_k;
};
