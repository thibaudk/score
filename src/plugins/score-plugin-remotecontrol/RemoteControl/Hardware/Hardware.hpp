#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include <score_plugin_remotecontrol_export.h>

#include "MidiController.hpp"

namespace RemoteControl
{

class SCORE_PLUGIN_REMOTECONTROL_EXPORT Hardware
    : public QObject
{
public:
  explicit Hardware(const score::DocumentContext& doc);

  void setupController() { ctrl.setup(); };

private:
  Explorer::DeviceDocumentPlugin& m_dev;

  MidiController ctrl;
};

}

#endif // HARDWARE_HPP
