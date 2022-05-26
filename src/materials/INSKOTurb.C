#include "INSKOTurb.h"
#include "Assembly.h"
#include "FEProblemBase.h"

registerMooseObject("iFlowApp", INSKOTurb);

InputParameters
INSKOTurb::validParams()
{
    InputParameters params = Material::validParams();
    params.addClassDescription("This is the material class used to compute some of the strong "
                               "residuals for the INS equations.");
    params.addParam<int>("model", 4, "湍流模型");
    params.addCoupledVar("kinetic", "kinetic");
    params.addCoupledVar("omega", "omega");
    return params;
}

INSKOTurb::INSKOTurb(const InputParameters &parameters)
    : Material(parameters),
      _model(getParam<int>("model")),
      _kinetic(coupledValue("kinetic")),
      _omega(coupledValue("omega")),
      _rho(getMaterialProperty<Real>("rho")),
      _mu_l(getMaterialProperty<Real>("mu_l")),
      _mu_turb(declareProperty<Real>("mu_turb")),
      _mu_eff(declareProperty<Real>("mu"))
{
}

void INSKOTurb::computeQpProperties()
{
    if (_model == 4)
    {
        _mu_turb[_qp] = std::max(0., _rho[_qp] * _kinetic[_qp] / _omega[_qp]);
        _mu_eff[_qp] = _mu_l[_qp] + _mu_turb[_qp];
    }
}