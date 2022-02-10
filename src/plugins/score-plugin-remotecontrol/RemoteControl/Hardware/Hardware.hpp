#ifndef HARDWARE_HPP
#define HARDWARE_HPP

//#include <QGraphicsItem>
#include <QPainter>

#include <score_plugin_remotecontrol_export.h>

#include "MidiController.hpp"

namespace RemoteControl
{

class SCORE_PLUGIN_REMOTECONTROL_EXPORT Hardware
    : public QObject
//    , public QGraphicsItem
{
  W_OBJECT(Hardware)

public:
  explicit Hardware(const score::DocumentContext& doc);

  void setupController();

private:
  Explorer::DeviceDocumentPlugin& m_dev;
  std::function<void(Controller::Comands, const bool&)> commandCallback;

  MidiController* ctl{nullptr};

  QImage* img;
  QPainter imgPainter;
};

}

#endif // HARDWARE_HPP
