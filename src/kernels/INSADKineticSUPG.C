//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "INSADKineticSUPG.h"

registerMooseObject("iFlowApp", INSADKineticSUPG);

InputParameters
INSADKineticSUPG::validParams()
{
  InputParameters params = ADKernelSUPG::validParams();
  params.addClassDescription("Kinetic SUPG");
  params.set<MaterialPropertyName>("tau_name") = "tau_kinetic";
  return params;
}

INSADKineticSUPG::INSADKineticSUPG(const InputParameters & parameters)
  : ADKernelSUPG(parameters),
    _kinetic_strong_residual(getADMaterialProperty<Real>("kinetic_strong_residual"))
{
}

ADReal
INSADKineticSUPG::precomputeQpStrongResidual()
{
  return _kinetic_strong_residual[_qp];
}
