#pragma once

#include "INSBase.h"

class INSTurbBase : public INSBase
{
public:
    static InputParameters validParams();

    INSTurbBase(const InputParameters &parameters);

    virtual ~INSTurbBase() {}

protected:
    virtual Real computeQpResidual();
    virtual Real computeQpJacobian();
    virtual Real computeQpOffDiagJacobian(unsigned jvar);
    virtual Real computeQpResidualViscousPart() = 0;
    virtual Real computeQpJacobianViscousPart() = 0;
    virtual Real computeQpOffDiagJacobianViscousPart(unsigned jvar) = 0;

    virtual Real computeQpPGResidual();
    virtual Real computeQpPGJacobian();

    virtual Real computeQpPGResidualallPart() = 0;
    virtual Real computeQpPGJacobianallPart() = 0;

    // k-w turbulence model require variables of kinetic and omega
    // baseline = 1;SST = 2;standard = 3;original = 4
    const int _model;
    //  Coupled variables
    const VariableValue &_kinetic;
    const VariableValue &_omega;

    // Gradients
    const VariableGradient &_grad_kinetic;
    const VariableGradient &_grad_omega;

    // Seconds
    const VariableSecond &_second_kinetic;
    const VariableSecond &_second_omega;

    // Time derivatives
    const VariableValue &_kinetic_dot;
    const VariableValue &_omega_dot;
    // SUPG需要的部分
    //是否对kinetic 和 omega 方程启用稳定效应
    bool _supg;

    //  Derivatives of time derivatives
    const VariableValue &_d_kinetic_dot_du;
    const VariableValue &_d_omega_dot_du;

    const MaterialProperty<Real> &_mu_turb;
    const MaterialProperty<Real> &_mu_l;
    virtual Real Prod();

    unsigned _w_var_number;
    //virtual Real computeQpOffDiagJacobian(unsigned jvar);
};