#pragma once
#include <score/plugins/settingsdelegate/SettingsDelegateView.hpp>

#include <RemoteControl/Settings/Model.hpp>
class QCheckBox;

namespace score
{
class FormWidget;
}
namespace RemoteControl
{
namespace Settings
{

class View : public score::GlobalSettingsView
{
  W_OBJECT(View)
public:
  View();
  void setNetEnabled(bool);
  void setHwEnabled(bool);

  void netEnabledChanged(bool b) W_SIGNAL(netEnabledChanged, b);
  void hwEnabledChanged(bool b) W_SIGNAL(hwEnabledChanged, b);

private:
  QWidget* getWidget() override;
  score::FormWidget* m_widg{};

  QCheckBox* m_netEnabled{};
  QCheckBox* m_hwEnabled{};
};

}
}
