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
      case 80:
        shift = message.bytes[2] > 0;
        break;
      case 29:
        if (message.bytes[2] > 0)
          onCommand(Controller::Play, shift);
        break;
      case 19:
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

    msg.erase(msg.begin() + 3, msg.end());

    msg[0] = 144;

    int notes[]{29, 19};
    int colors[]{64, 9};

    for (int i = 0; i < 2; i++)
    {
      msg[1] = notes[i];
      msg[2] = colors[i];
      m_output.send_message(msg);
    }

    openPortByName(m_input, deviceName);
  }
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
