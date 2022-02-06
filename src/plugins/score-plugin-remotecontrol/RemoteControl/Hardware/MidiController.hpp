#ifndef MIDICONTROLLER_HPP
#define MIDICONTROLLER_HPP

#include <Scenario/Application/ScenarioActions.hpp>
#include <score/actions/ActionManager.hpp>

#include <libremidi/libremidi.hpp>

#include "Controller.hpp"

namespace Explorer
{
class DeviceDocumentPlugin;
}

namespace RemoteControl
{

class MidiController : public Controller
{
public:
  MidiController();

  ~MidiController();

  void setup(const QString& deviceName = " Launchpad Pro Standalone Port");

  std::function<void(Controller::Comands, const bool&)> onCommand;

private:
  bool shift{false};

  template <typename T>
  void openPortByName(T& libremidi, const QString& deviceName);

  libremidi::midi_out m_output;
  libremidi::midi_in m_input;
  std::vector<unsigned char> msg{240, 0, 32, 41, 2, 16, 44, 3, 247};
};

}

#endif // MIDICONTROLLER_HPP
