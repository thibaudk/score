#include "View.hpp"

#include <score/widgets/FormWidget.hpp>

#include <QCheckBox>
#include <QFormLayout>

#include <wobjectimpl.h>
W_OBJECT_IMPL(RemoteControl::Settings::View)
namespace RemoteControl
{
namespace Settings
{

View::View()
{
  m_widg = new score::FormWidget{tr("Remote control")};
  auto lay = m_widg->layout();

  {
    m_netEnabled = new QCheckBox{tr("Enable on network")};

    connect(m_netEnabled, &QCheckBox::stateChanged, this, [&](int t) {
      switch (t)
      {
        case Qt::Unchecked:
          netEnabledChanged(false);
          break;
        case Qt::Checked:
          netEnabledChanged(true);
          break;
        default:
          break;
      }
    });

    lay->addRow(m_netEnabled);
  }

  {
    m_hwEnabled = new QCheckBox{tr("Enable on hardware")};

    connect(m_hwEnabled, &QCheckBox::stateChanged, this, [&](int t) {
      if(t == Qt::Unchecked)
        hwEnabledChanged(false);
      else
        hwEnabledChanged(true);
    });

    lay->addRow(m_hwEnabled);
  }
}

void View::setNetEnabled(bool val)
{
  switch (m_netEnabled->checkState())
  {
    case Qt::Unchecked:
      if (val)
        m_netEnabled->setChecked(true);
      break;
    case Qt::Checked:
      if (!val)
        m_netEnabled->setChecked(false);
      break;
    default:
      break;
  }
}

void View::setHwEnabled(bool val)
{
  if (val)
  {
    if (m_hwEnabled->checkState() == Qt::Unchecked)
      m_hwEnabled->setChecked(true);
  }
  else
  {
    if (m_hwEnabled->checkState() == Qt::Checked)
      m_hwEnabled->setChecked(false);
  }
}

QWidget* View::getWidget()
{
  return m_widg;
}

}
}
