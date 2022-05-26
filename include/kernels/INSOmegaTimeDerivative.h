#pragma once

#include "TimeDerivative.h"

// Forward Declarations

/**
 * This class computes the time derivative for the incompressible
 * Navier-Stokes momentum equation.  Could instead use CoefTimeDerivative
 * for this.
 */
class INSOmegaTimeDerivative : public TimeDerivative
{
public:
    static InputParameters validParams();

    INSOmegaTimeDerivative(const InputParameters &parameters);

    virtual ~INSOmegaTimeDerivative() {}

protected:
    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();
    virtual Real computeQpOffDiagJacobian(unsigned jvar);

    // Parameters
    const MaterialProperty<Real> &_rho;
};