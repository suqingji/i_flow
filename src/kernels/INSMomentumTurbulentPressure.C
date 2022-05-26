#include "INSMomentumTurbulentPressure.h"

template <>
InputParameters
validParams<INSMomentumTurbulentPressure>()
{
  InputParameters params = validParams<Kernel>();

  params.addClassDescription("Models the turbulent pressure term of the INS momentum equation.");

  // Coupled variables
  params.addRequiredCoupledVar("kin", "The turbulent kinetic energy");

  // Required parameters
  params.addRequiredParam<bool>("integrate_by_parts", "Whether to integrate this term by parts");
  params.addRequiredParam<unsigned>(
      "component",
      "0,1,2 depending on if we are solving the x,y,z component of the momentum equation");

  // Optional parameters
  params.addParam<MaterialPropertyName>("rho_name", "rho", "Name of density");

  return params;
}

INSMomentumTurbulentPressure::INSMomentumTurbulentPressure(const InputParameters & parameters)
  : Kernel(parameters),

    // Coupled variables
    _kin(coupledValue("kin")),
    _grad_kin(coupledGradient("kin")),
    _kin_id(coupled("kin")),

    // Parameters
    _integrate_by_parts(getParam<bool>("integrate_by_parts")),
    _component(getParam<unsigned>("component")),

    // Material Properties
    _rho(getMaterialProperty<Real>("rho_name"))
{
}

Real
INSMomentumTurbulentPressure::computeQpResidual()
{
  if (_integrate_by_parts)
    return -_grad_test[_i][_qp](_component) * 2. / 3. * _rho[_qp] * _kin[_qp];
  else
    return _test[_i][_qp] * 2. / 3. * _rho[_qp] * _grad_kin[_qp](_component);
}

Real
INSMomentumTurbulentPressure::computeQpJacobian()
{
  return 0.;
}

Real
INSMomentumTurbulentPressure::computeQpOffDiagJacobian(unsigned jvar)
{
  if (jvar == _kin_id)
  {
    if (_integrate_by_parts)
      return -_grad_test[_i][_qp](_component) * 2. / 3. * _rho[_qp] * _phi[_j][_qp];
    else
      return _test[_i][_qp] * 2. / 3. * _rho[_qp] * _grad_phi[_j][_qp](_component);
  }

  else
    return 0;
}