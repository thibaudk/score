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

  std::function<void(Controller::Commands, const bool&)> on_command;

  void setup(const QString& deviceName = " Launchpad Pro Standalone Port");
  void set_tile_from_rgb(int index, const QRgb& value);

  int grid_width{};
  int grid_heigt{};

private:
  bool shift{false};

  void open_port_by_name(const QString& deviceName);

  libremidi::midi_out m_output;
  libremidi::midi_in m_input;
};

}

#endif // MIDICONTROLLER_HPP
