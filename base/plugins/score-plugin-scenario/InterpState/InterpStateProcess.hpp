#pragma once
#include <Curve/Process/CurveProcessModel.hpp>
#include <State/Message.hpp>
#include <Process/ProcessMetadata.hpp>
#include <score_plugin_scenario_export.h>
namespace InterpState
{
class ProcessModel;
}

PROCESS_METADATA(
    ,
    InterpState::ProcessModel,
    "09fa6f72-55d5-4fee-8bc7-6f983c2e62d8",
    "InterpState",
    "State interpolation",
    "Automations",
    (QStringList{"Curve", "Automation"}),
    Process::ProcessFlags::SupportsTemporal)
namespace InterpState
{
class SCORE_PLUGIN_SCENARIO_EXPORT ProcessModel final
    : public Curve::CurveProcessModel
{
  SCORE_SERIALIZE_FRIENDS
  PROCESS_METADATA_IMPL(InterpState::ProcessModel)

  W_OBJECT(ProcessModel)

public:
  ProcessModel(
      const TimeVal& duration,
      const Id<Process::ProcessModel>& id,
      QObject* parent);

  ~ProcessModel() override;

  template <typename Impl>
  ProcessModel(Impl& vis, QObject* parent)
      : CurveProcessModel{vis, parent}
  {
    vis.writeTo(*this);
  }

  QString prettyName() const override;

  State::MessageList startMessages() const;
  State::MessageList endMessages() const;

private:
  //// ProcessModel ////
  void setDurationAndScale(const TimeVal& newDuration) override;
  void setDurationAndGrow(const TimeVal& newDuration) override;
  void setDurationAndShrink(const TimeVal& newDuration) override;
  bool contentHasDuration() const override;
  TimeVal contentDuration() const override;
};
}