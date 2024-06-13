#include <wobjectimpl.h>

#include <score/application/ApplicationContext.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <score/actions/ActionManager.hpp>
#include <Scenario/Application/ScenarioActions.hpp>

#include "Hardware.hpp"

namespace RemoteControl {
W_OBJECT_IMPL(Hardware)

Hardware::Hardware(const score::DocumentContext& doc)
  : m_dev{doc.plugin<Explorer::DeviceDocumentPlugin>()}
  , img{new QImage{}}
  , imgPainter{img}
{
  commandCallback = [&doc](Controller::Commands com, const bool& shift) {
    switch (com)
    {
    case Controller::Play:
      if (shift)
        doc.app.actions.action<Actions::PlayGlobal>().action()->trigger();
      else
        doc.app.actions.action<Actions::Play>().action()->trigger();
      break;
    case Controller::Stop:
      if (shift)
        doc.app.actions.action<Actions::Reinitialize>().action()->trigger();
      else
        doc.app.actions.action<Actions::Stop>().action()->trigger();
      break;
    default:
      break;
    }
  };
}

void Hardware::setupController()
{
  ctl = new MidiController{};
  ctl->on_command = commandCallback;
  ctl->setup();
  *img = QImage(8, 8, QImage::Format_RGB16);

  qDebug() << img->pixel(0, 0);

  imgPainter.begin(img);
  imgPainter.scale(1, 1);

  imgPainter.setPen(Qt::blue);
  imgPainter.drawPoint(0, 0);
  imgPainter.end();

  qDebug() << img->pixel(0, 0);
}

}
