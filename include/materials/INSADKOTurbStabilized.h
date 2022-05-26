#pragma once

#include "InputParameters.h"
#include "NonlinearSystemBase.h"
#include "FEProblemBase.h"
#include "MaterialProperty.h"
#include "MooseArray.h"

#include "libmesh/elem.h"

#include <vector>

class INSADMaterial;
class INSADKOTurb;

#include "INSADTauMaterial.h"
#include "INSADKOTurb.h"

class INSADKOTurbStabilized : public INSADTauMaterialTempl<INSADKOTurb>
{
public:
  static InputParameters validParams();

  INSADKOTurbStabilized(const InputParameters &parameters);

protected:
  virtual void computeQpProperties() override;

  const ADVariableSecond &_second_kinetic;
  ADMaterialProperty<Real> &_tau_kinetic;
  ADMaterialProperty<Real> &_kinetic_strong_residual;
  using INSADTauMaterialTempl<INSADKOTurb>::_mu_k;
  using INSADTauMaterialTempl<INSADKOTurb>::_kinetic_advective_strong_residual;
  using INSADTauMaterialTempl<INSADKOTurb>::_kinetic_td_strong_residual;
  using INSADTauMaterialTempl<INSADKOTurb>::_kinetic_source_strong_residual;
  // using INSADTauMaterialTempl<INSADKOTurb>::_has_kinetic_transient;

  const ADVariableSecond &_second_omega;
  ADMaterialProperty<Real> &_tau_omega;
  ADMaterialProperty<Real> &_omega_strong_residual;
  using INSADTauMaterialTempl<INSADKOTurb>::_mu_omega;
  using INSADTauMaterialTempl<INSADKOTurb>::_omega_advective_strong_residual;
  using INSADTauMaterialTempl<INSADKOTurb>::_omega_td_strong_residual;
  using INSADTauMaterialTempl<INSADKOTurb>::_omega_source_strong_residual;
  // using INSADTauMaterialTempl<INSADKOTurb>::_has_omega_transient;

  using INSADTauMaterialTempl<INSADKOTurb>::_mu_turb;
};