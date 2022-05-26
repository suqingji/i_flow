#pragma once

#include "ADKernelValue.h"

/**
 * This class computes the residual and Jacobian contributions for
 * temperature advection
 */
class INSADKineticAdvection : public ADKernelValue
{
public:
  static InputParameters validParams();

  INSADKineticAdvection(const InputParameters &parameters);

protected:
  virtual ADReal precomputeQpResidual() override;

  const ADMaterialProperty<Real> &_kinetic_advective_strong_residual;
  const ADMaterialProperty<Real> &_kinetic_source_residual;
};
