#include "INSKineticTimeDerivative.h"

registerMooseObject("iFlowApp", INSKineticTimeDerivative);

InputParameters
INSKineticTimeDerivative::validParams()
{
    InputParameters params = TimeDerivative::validParams();
    params.addClassDescription("This class computes the time derivative for the incompressible "
                               "Navier-Stokes turb equation.");
    params.addParam<MaterialPropertyName>("rho", "rho", "density name");
    return params;
}

INSKineticTimeDerivative::INSKineticTimeDerivative(const InputParameters &parameters)
    : TimeDerivative(parameters), _rho(getMaterialProperty<Real>("rho"))
{
}

Real INSKineticTimeDerivative::computeQpResidual()
{
    return _rho[_qp] * TimeDerivative::computeQpResidual();
}

Real INSKineticTimeDerivative::computeQpJacobian()
{
    return _rho[_qp] * TimeDerivative::computeQpJacobian();
}

Real INSKineticTimeDerivative::computeQpOffDiagJacobian(unsigned)
{
    return 0.;
}