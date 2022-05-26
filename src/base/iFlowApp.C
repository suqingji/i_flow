#include "iFlowApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
iFlowApp::validParams()
{
  InputParameters params = MooseApp::validParams();

  // Do not use legacy material output, i.e., output properties on INITIAL as well as TIMESTEP_END
  params.set<bool>("use_legacy_material_output") = false;

  return params;
}

iFlowApp::iFlowApp(InputParameters parameters) : MooseApp(parameters)
{
  iFlowApp::registerAll(_factory, _action_factory, _syntax);
}

iFlowApp::~iFlowApp() {}

void
iFlowApp::registerAll(Factory & f, ActionFactory & af, Syntax & syntax)
{
  ModulesApp::registerAll(f, af, syntax);
  Registry::registerObjectsTo(f, {"iFlowApp"});
  Registry::registerActionsTo(af, {"iFlowApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
iFlowApp::registerApps()
{
  registerApp(iFlowApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
iFlowApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  iFlowApp::registerAll(f, af, s);
}
extern "C" void
iFlowApp__registerApps()
{
  iFlowApp::registerApps();
}
