#pragma once

#include "INSADMaterial.h"

/**
 * Computes properties needed for stabilized formulations of the mass, momentum, and energy
 * equations
 */
class INSADKOTurb : public INSADMaterial
{
public:
    static InputParameters validParams();

    INSADKOTurb(const InputParameters &parameters);

    void subdomainSetup() override;

    ADReal tensorReduce(const ADRealTensorValue &l, const ADRealTensorValue &r)
    {
        ADReal v = 0;
        for (size_t i = 0; i < 3; i++)
            for (size_t j = 0; j < 3; j++)
                v += l(i, j) * r(i, j);

        return v;
    }

protected:
    void computeQpProperties() override;

    const ADVariableValue &_kinetic;
    const ADVariableGradient &_grad_kinetic;
    const ADVariableValue *_kinetic_dot;
    MaterialProperty<Real> &_mu_k;

    ADMaterialProperty<Real> &_kinetic_advective_strong_residual;
    ADMaterialProperty<Real> &_kinetic_td_strong_residual;
    ADMaterialProperty<Real> &_kinetic_source_strong_residual;

    const ADVariableValue &_omega;
    const ADVariableGradient &_grad_omega;
    const ADVariableValue *_omega_dot;
    MaterialProperty<Real> &_mu_omega;

    ADMaterialProperty<Real> &_omega_advective_strong_residual;
    ADMaterialProperty<Real> &_omega_td_strong_residual;
    ADMaterialProperty<Real> &_omega_source_strong_residual;

    ADMaterialProperty<Real> &_mu_turb;

    const ADMaterialProperty<Real> &_mu_l;
    ADMaterialProperty<Real> &_mu_eff;
    /// Whether the PD equation is transient
    //在非稳态项中设置为true这里引用，参考has_erengy_transient
    // bool _has_kinetic_transient;
    // bool _has_omega_transient;
};