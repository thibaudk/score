#include "Presenter.hpp"

#include "Model.hpp"
#include "View.hpp"

#include <score/command/Command.hpp>
#include <score/command/Dispatchers/ICommandDispatcher.hpp>
#include <score/command/SettingsCommand.hpp>
#include <score/widgets/SetIcons.hpp>

#include <QApplication>
#include <QStyle>

namespace RemoteControl
{
namespace Settings
{
Presenter::Presenter(Model& m, View& v, QObject* parent)
    : score::GlobalSettingsPresenter{m, v, parent}
{
  {
    // view -> model
    con(v, &View::netEnabledChanged, this, [&](auto val) {
      if (val != m.getNetEnabled())
      {
        m_disp.submit<SetModelNetEnabled>(this->model(this), val);
      }
    });

    con(v, &View::hwEnabledChanged, this, [&](auto val) {
      if (val != m.getHwEnabled())
      {
        m_disp.submit<SetModelHwEnabled>(this->model(this), val);
      }
    });

    // model -> view
    con(m, &Model::NetEnabledChanged, &v, &View::setNetEnabled);
    con(m, &Model::HwEnabledChanged, &v, &View::setHwEnabled);

    // initial value
    v.setNetEnabled(m.getNetEnabled());
    v.setHwEnabled(m.getHwEnabled());
  }
}

QString Presenter::settingsName()
{
  return tr("Remote control");
}

QIcon Presenter::settingsIcon()
{
  return makeIcons(
      QStringLiteral(":/icons/settings_remote_control_on.png"),
      QStringLiteral(":/icons/settings_remote_control_off.png"),
      QStringLiteral(":/icons/settings_remote_control_off.png"));
}

}
}
