#include <libremidi/libremidi.hpp>

#include "MidiController.hpp"

namespace RemoteControl {

MidiController::MidiController()
  : m_output{}
  , m_input{}
{
  m_input.set_callback([this](const libremidi::message& message) {

    if (message.get_message_type() == libremidi::message_type::CONTROL_CHANGE)
      switch (message.bytes[1])
      {
      case SHIFT:
        shift = message.bytes[2] > 0;
        break;
      case PLAY:
        if (message.bytes[2] > 0)
          onCommand(Controller::Play, shift);
        break;
      case STOP:
        if (message.bytes[2] > 0)
          onCommand(Controller::Stop, shift);
        break;
      default:
        break;
      }
  });
}

MidiController::~MidiController()
{
  m_input.close_port();
  m_output.close_port();
}

void MidiController::setup(const QString& deviceName)
{
  openPortByName(m_output, deviceName);

  if (m_output.is_port_open())
  {
    m_output.send_message(msg);

    u_int8_t notes[]{SHIFT, PLAY, STOP};
    u_int8_t colors[]{WHITE, GREEN, ORANGE};

    for (int i = 0; i < 3; i++)
    {
      m_output.send_message(msg.note_on(1, notes[i], colors[i]));
    }

    openPortByName(m_input, deviceName);

    gridWidth = NUM_ROWS;
    gridHeigt = NUM_COLUMNS;
  }
}

void MidiController::setTileFromRgb(int index, const QRgb& value)
{

}

template<typename T>
void MidiController::openPortByName(T& libremidi, const QString& deviceName)
{
  int n_ports = libremidi.get_port_count();

  if (n_ports == 0)
    return;

  for (int i = 0; i < n_ports; i++)
  {
    if (deviceName == QString().fromStdString(libremidi.get_port_name(i)).split(":").back())
      libremidi.open_port(i);
  }
}

}
