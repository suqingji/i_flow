#pragma once


#include "Kernel.h"

class INSMomentumTurbulentPressure : public Kernel
{
public:
  INSMomentumTurbulentPressure(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned jvar) override;

  const VariableValue & _kin;
  const VariableGradient & _grad_kin;
  unsigned _kin_id;

  bool _integrate_by_parts;
  unsigned _component;

  const MaterialProperty<Real> & _rho;
};