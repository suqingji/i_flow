#pragma once

#include "ADDiffusion.h"

class INSADOmegaDiffusion : public ADDiffusion
{
public:
  static InputParameters validParams();

  INSADOmegaDiffusion(const InputParameters &parameters);

protected:
  virtual ADRealVectorValue precomputeQpResidual() override;

  const MaterialProperty<Real> &_mu_omega;
};
