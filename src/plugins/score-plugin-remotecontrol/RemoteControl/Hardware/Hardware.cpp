#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <score/actions/ActionManager.hpp>
#include <Scenario/Application/ScenarioActions.hpp>

#include "Hardware.hpp"

namespace RemoteControl {


Hardware::Hardware(const score::DocumentContext& doc)
  : m_dev{doc.plugin<Explorer::DeviceDocumentPlugin>()}
  , ctrl{}
{
  ctrl.onCommand = [&doc](Controller::Comands com, const bool& shift) {
    switch (com)
    {
    case Controller::Play:
      if (shift)
        doc.app.actions.action<Actions::PlayGlobal>().action()->trigger();
      else
        doc.app.actions.action<Actions::Play>().action()->trigger();
      break;
    case Controller::Stop:
      if (shift)
        doc.app.actions.action<Actions::Reinitialize>().action()->trigger();
      else
        doc.app.actions.action<Actions::Stop>().action()->trigger();
      break;
    default:
      break;
    }
  };
}

}
