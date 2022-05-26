#pragma once

#include "ADKernelValue.h"

/**
 * This class computes the residual and Jacobian contributions for
 * temperature advection
 */
class INSADOmegaAdvection : public ADKernelValue
{
public:
  static InputParameters validParams();

  INSADOmegaAdvection(const InputParameters &parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const ADMaterialProperty<Real> &_omega_advective_strong_residual;
  const ADMaterialProperty<Real> &_omega_source_residual;
};
