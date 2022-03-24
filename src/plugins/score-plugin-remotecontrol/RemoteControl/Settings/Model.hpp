#pragma once
#include <score/plugins/settingsdelegate/SettingsDelegateModel.hpp>

#include <score_plugin_remotecontrol_export.h>

namespace RemoteControl
{
namespace Settings
{
class SCORE_PLUGIN_REMOTECONTROL_EXPORT Model : public score::SettingsDelegateModel
{
  W_OBJECT(Model)
  bool m_NetEnabled = false;
  bool m_HwEnabled = false;



public:
  Model(QSettings& set, const score::ApplicationContext& ctx);

  SCORE_SETTINGS_PARAMETER_HPP(
      SCORE_PLUGIN_REMOTECONTROL_EXPORT,
      bool,
      NetEnabled)
  SCORE_SETTINGS_PARAMETER_HPP(
      SCORE_PLUGIN_REMOTECONTROL_EXPORT,
      bool,
      HwEnabled)
};

SCORE_SETTINGS_PARAMETER(Model, NetEnabled)
SCORE_SETTINGS_PARAMETER(Model, HwEnabled)
}
}
