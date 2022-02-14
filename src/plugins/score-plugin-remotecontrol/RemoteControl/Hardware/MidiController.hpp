#ifndef MIDICONTROLLER_HPP
#define MIDICONTROLLER_HPP

#include <Scenario/Application/ScenarioActions.hpp>
#include <score/actions/ActionManager.hpp>

#include <libremidi/libremidi.hpp>

#include "Controller.hpp"

// Macros to temporarly act as the device description file
#define PROGRAMER_MODE {240, 0, 32, 41, 2, 16, 44, 3, 247}

#define SHIFT 80
#define PLAY 29
#define STOP 19

#define WHITE 2
#define GREEN 64
#define ORANGE 9

// Rows & Columns
#define NUM_ROWS 8
#define NUM_COLUMNS 8

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

  std::function<void(Controller::Commands, const bool&)> onCommand;

  void setup(const QString& deviceName = " Launchpad Pro Standalone Port");
  void setTileFromRgb(int index, const QRgb& value);

  int gridWidth{};
  int gridHeigt{};

private:
  bool shift{false};

  template <typename T>
  void openPortByName(T& libremidi, const QString& deviceName);

  libremidi::midi_out m_output;
  libremidi::midi_in m_input;
  libremidi::message msg PROGRAMER_MODE;
};

}

#endif // MIDICONTROLLER_HPP
