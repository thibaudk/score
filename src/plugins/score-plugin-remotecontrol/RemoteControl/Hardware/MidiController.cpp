#include <libremidi/libremidi.hpp>

#include "MidiController.hpp"

namespace RemoteControl {

MidiController::MidiController()
    : m_output{}
    , m_input{{.on_message = [this] (const libremidi::message& message)
{
  if (message.get_message_type() == libremidi::message_type::CONTROL_CHANGE)
    switch (message.bytes[1])
    {
      case SHIFT:
        shift = message.bytes[2] > 0;
        break;
      case PLAY:
        if (message.bytes[2] > 0)
          on_command(Controller::Play, shift);
        break;
      case STOP:
        if (message.bytes[2] > 0)
          on_command(Controller::Stop, shift);
        break;
      default:
        break;
    }
},
              .ignore_sysex = false,
              .ignore_timing = false,
              .ignore_sensing = false,
          }
      }
{}

MidiController::~MidiController()
{
  m_input.close_port();
  m_output.close_port();
}

void MidiController::setup(const QString& deviceName)
{
  open_port_by_name(deviceName);

  if (m_output.is_port_open())
  {
    using namespace libremidi;
    m_output.send_message(message PROGRAMER_MODE);

    u_int8_t notes[]{SHIFT, PLAY, STOP};
    u_int8_t colors[]{WHITE, GREEN, ORANGE};

    for (int i = 0; i < 3; i++)
    {
      m_output.send_message(channel_events::note_on(1, notes[i], colors[i]));
    }

    grid_width = NUM_ROWS;
    grid_heigt = NUM_COLUMNS;
  }
}

void MidiController::set_tile_from_rgb(int index, const QRgb& value)
{
}

void MidiController::open_port_by_name(const QString& deviceName)
{
  libremidi::observer obs;

  for (const auto& input : obs.get_input_ports())
  {
    if (deviceName == QString::fromStdString(input.port_name).split(":").back())
      m_input.open_port(input);
  }

  // TODO : error handling here
  if (m_input.is_port_connected()) return;

  auto outputs = obs.get_output_ports();

  for (const auto& output : obs.get_output_ports())
  {
    if (deviceName == QString::fromStdString(output.port_name).split(":").back())
      m_output.open_port(output);
  }

  // TODO : error handling here
  if (m_output.is_port_connected()) return;
}

}
