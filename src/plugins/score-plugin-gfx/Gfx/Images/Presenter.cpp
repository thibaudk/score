#include <Process/Dataflow/Port.hpp>
#include <Process/Dataflow/PortItem.hpp>
#include <Process/Style/Pixmaps.hpp>

#include <score/command/Dispatchers/CommandDispatcher.hpp>
#include <score/command/Dispatchers/MacroCommandDispatcher.hpp>
#include <score/graphics/GraphicWidgets.hpp>
#include <score/model/path/PathSerialization.hpp>
#include <score/tools/IdentifierGeneration.hpp>

#include <Control/DefaultEffectItem.hpp>
#include <Effect/EffectLayout.hpp>
#include <Gfx/Images/Presenter.hpp>
#include <Gfx/Images/Process.hpp>
#include <Gfx/Images/View.hpp>

namespace Gfx::Images
{
struct Presenter::Port
{
  score::EmptyRectItem* root;
  Dataflow::PortItem* port;
  QRectF rect;
};
Presenter::Presenter(const Model& layer, View* view, const Process::Context& ctx, QObject* parent)
    : Process::LayerPresenter{layer, view, ctx, parent}, m_view{view}
{
  auto& portFactory = ctx.app.interfaces<Process::PortFactoryList>();
  for (auto& e : layer.inlets())
  {
    auto inlet = qobject_cast<Process::ControlInlet*>(e);
    if (!inlet)
      continue;

    setupInlet(*inlet, portFactory, ctx);
  }
}

void Presenter::setWidth(qreal val, qreal defaultWidth)
{
  m_view->setWidth(val);
}

void Presenter::setHeight(qreal val)
{
  m_view->setHeight(val);
}

void Presenter::putToFront()
{
  m_view->setOpacity(1);
}

void Presenter::putBehind()
{
  m_view->setOpacity(0.2);
}

void Presenter::on_zoomRatioChanged(ZoomRatio) { }

void Presenter::parentGeometryChanged() { }

void Presenter::setupInlet(
    Process::ControlInlet& port,
    const Process::PortFactoryList& portFactory,
    const Process::Context& doc)
{
  // Main item creation
  int i = m_ports.size();

  auto csetup = Process::controlSetup(
      [](auto& factory, auto& inlet, const auto& doc, auto item, auto parent) {
        return factory.makeItem(inlet, doc, item, parent);
      },
      [](auto& factory, auto& inlet, const auto& doc, auto item, auto parent) {
        return factory.makeControlItem(inlet, doc, item, parent);
      },
      [&](int j) { return m_ports[j].rect.size(); },
      [&] { return port.customData(); });
  auto [item, portItem, widg, lab, itemRect]
      = Process::createControl(i, csetup, port, portFactory, doc, m_view, this);

  m_ports.push_back(Port{item, portItem, itemRect});
  // TODO updateRect();

  // TODO con(inlet, &Process::ControlInlet::domainChanged, this, [this,
  // &inlet] {
  // TODO   on_controlRemoved(inlet);
  // TODO   on_controlAdded(inlet.id());
  // TODO });
}
}
