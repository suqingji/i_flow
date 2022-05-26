#pragma once

#include "INSTurbBase.h"

// Forward Declarations

/**
 * This class computes momentum equation residual and Jacobian viscous
 * contributions for the "Laplacian" form of the governing equations.
 */
class INSOmega : public INSTurbBase
{
public:
    static InputParameters validParams();

    INSOmega(const InputParameters &parameters);

    virtual ~INSOmega() {}

protected:
    virtual Real computeQpResidualViscousPart() override;
    virtual Real computeQpJacobianViscousPart() override;
    virtual Real computeQpOffDiagJacobianViscousPart(unsigned jvar) override;

    virtual Real computeQpPGResidualallPart() override;
    virtual Real computeQpPGJacobianallPart() override;
    // SUPG的
    virtual Real tau_omega();
    // Time derivatives
    const VariableValue &_u_dot;
    // Derivatives of time derivatives
    const VariableValue &_du_dot_du;
    // Seconds
    const VariableSecond &_second_u;
};