//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ADKernelSUPG.h"

/**
 * This class computes the residual and Jacobian contributions for
 * kinetic equation SUPG stabilization
 */
class INSADKineticSUPG : public ADKernelSUPG
{
public:
  static InputParameters validParams();

  INSADKineticSUPG(const InputParameters & parameters);

protected:
  ADReal precomputeQpStrongResidual() override;

  const ADMaterialProperty<Real> & _kinetic_strong_residual;
};
