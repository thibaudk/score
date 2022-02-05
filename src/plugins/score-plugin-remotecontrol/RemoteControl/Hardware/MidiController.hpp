#ifndef MIDICONTROLLER_HPP
#define MIDICONTROLLER_HPP

#include <score_plugin_remotecontrol_export.h>

#include <libremidi/libremidi.hpp>

namespace Explorer
{
class DeviceDocumentPlugin;
}

namespace RemoteControl
{

class SCORE_PLUGIN_REMOTECONTROL_EXPORT MidiController
    : public QObject
    , Nano::Observer
{
public:
  MidiController(const score::DocumentContext& doc);

  ~MidiController() = default;

private:
  Explorer::DeviceDocumentPlugin& m_dev;

  void setup();

  template <typename T>
  void openPortByName(T& libremidi, const QString& deviceName = " Launchpad Pro Standalone Port");

  libremidi::midi_out m_output;
  libremidi::midi_in m_input;
  std::vector<unsigned char> msg{240, 0, 32, 41, 2, 16, 44, 3, 247};
};

}

#endif // MIDICONTROLLER_HPP
