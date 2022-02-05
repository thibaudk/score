#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>

#include <libremidi/libremidi.hpp>

#include "MidiController.hpp"

namespace RemoteControl {

MidiController::MidiController(const score::DocumentContext& doc)
  : m_dev{doc.plugin<Explorer::DeviceDocumentPlugin>()}
  , m_output{}
  , m_input{}
{
  setup();
}

void MidiController::setup()
{
  openPortByName(m_output);

  if (m_output.is_port_open())
  {
    m_output.send_message(msg);

    msg.erase(msg.begin() + 3, msg.end());

    msg[0] = 144;

    int notes[]{49, 39, 29, 19};
    int colors[]{64, 64, 9, 9};

    for (int i = 0; i < 4; i++)
    {
      msg[1] = notes[i];
      msg[2] = 0;
      m_output.send_message(msg);
      msg[1] = notes[i];
      msg[2] = colors[i];
      m_output.send_message(msg);
    }

    openPortByName(m_input);
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
