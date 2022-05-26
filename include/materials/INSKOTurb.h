#pragma once

#include "Material.h"

class INSKOTurb : public Material
{
public:
    static InputParameters validParams();

    INSKOTurb(const InputParameters &parameters);

    // void subdomainSetup() override;

protected:
    virtual void computeQpProperties() override;
    const int _model;
    const VariableValue &_kinetic;
    const VariableValue &_omega;

    const MaterialProperty<Real> &_rho;
    const MaterialProperty<Real> &_mu_l;
    MaterialProperty<Real> &_mu_turb;
    MaterialProperty<Real> &_mu_eff;
};