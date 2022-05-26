#include "INSOmegaTimeDerivative.h"

registerMooseObject("iFlowApp", INSOmegaTimeDerivative);

InputParameters
INSOmegaTimeDerivative::validParams()
{
    InputParameters params = TimeDerivative::validParams();
    params.addClassDescription("This class computes the time derivative for the incompressible "
                               "Navier-Stokes turb equation.");
    params.addParam<MaterialPropertyName>("rho", "rho", "density name");
    return params;
}

INSOmegaTimeDerivative::INSOmegaTimeDerivative(const InputParameters &parameters)
    : TimeDerivative(parameters), _rho(getMaterialProperty<Real>("rho"))
{
}

Real INSOmegaTimeDerivative::computeQpResidual()
{
    return _rho[_qp] * TimeDerivative::computeQpResidual();
}

Real INSOmegaTimeDerivative::computeQpJacobian()
{
    return _rho[_qp] * TimeDerivative::computeQpJacobian();
}

Real INSOmegaTimeDerivative::computeQpOffDiagJacobian(unsigned)
{
    return 0.;
}