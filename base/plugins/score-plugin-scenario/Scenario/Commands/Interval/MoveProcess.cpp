#include "MoveProcess.hpp"

#include <Scenario/Document/Interval/IntervalModel.hpp>
#include <Dataflow/Commands/CableHelpers.hpp>
#include <ossia/detail/algorithms.hpp>
#include <score/model/path/ObjectPath.hpp>
#include <score/model/path/PathSerialization.hpp>
#include <score/tools/IdentifierGeneration.hpp>
#include <Scenario/Process/ScenarioInterface.hpp>
#include <Scenario/Process/Algorithms/ProcessPolicy.hpp>
#include <Scenario/Process/Algorithms/Accessors.hpp>
#include <ossia/detail/pod_vector.hpp>
#include <Scenario/Settings/ScenarioSettingsModel.hpp>
#include <score/document/ChangeId.hpp>

namespace Scenario::Command
{

MoveProcess::MoveProcess(
      const IntervalModel& src
      , const IntervalModel& tgt
      , Id<Process::ProcessModel> processId):
    m_src{src}
  , m_tgt{tgt}
  , m_oldId{processId}
  , m_newId{getStrongId(tgt.processes)}
  , m_oldSmall{src.smallView()}
  , m_oldFull{src.fullView()}
{
  int i = 0;
  for(auto& e : src.fullView())
  {
    if(e.process == m_oldId)
    {
      m_oldPos = i;
      break;
    }
    i++;
  }
}

static
bool startsWith(const ObjectPath& object, const ObjectPath& parent)
{
  if(object.vec().size() < parent.vec().size())
    return false;

  for(std::size_t i = 0; i < parent.vec().size(); i++)
  {
    if(!(object.vec()[i] == parent.vec()[i]))
      return false;
  }

  return true;
}

static
void moveProcess(Scenario::IntervalModel& src
                 , Scenario::IntervalModel& tgt
                 , const Id<Process::ProcessModel>& old_id
                 , const Id<Process::ProcessModel>& new_id
                 , const score::DocumentContext& ctx
                 )
{
  Process::ProcessModel& proc = src.processes.at(old_id);
  auto cables = Dataflow::saveCables({&proc}, ctx);
  Dataflow::removeCables(cables, ctx);

  const auto old_path = score::IDocument::unsafe_path(proc);

  // 1. Remove the process from the old interval
  {
    ossia::int_vector slots_to_remove;
    int i = 0;
    for (const Slot& slot : src.smallView())
    {
      if (slot.processes.size() == 1 && slot.processes[0] == proc.id())
        slots_to_remove.push_back(i);
      i++;
    }

    EraseProcess(src, proc.id());

    for(auto it = slots_to_remove.rbegin(); it != slots_to_remove.rend(); ++it)
      src.removeSlot(*it);
  }

  // 2. Add the process to its new parent
  {
    proc.setParent(&tgt);
    score::IDocument::changeObjectId(proc, new_id);

    AddProcess(tgt, &proc);
  }

  // 3. Re-add the cables
  {
    const auto new_path = score::IDocument::path(proc).unsafePath();

    for(auto& c : cables)
    {
      if(auto& p = c.second.source.unsafePath(); startsWith(p, old_path))
        replacePathPart(old_path, new_path, p);

      if(auto& p = c.second.sink.unsafePath(); startsWith(p, old_path))
        replacePathPart(old_path, new_path, p);
    }

    Dataflow::restoreCables(cables, ctx);
  }
}

void MoveProcess::undo(const score::DocumentContext& ctx) const
{
  auto& src = m_src.find(ctx);
  moveProcess(m_tgt.find(ctx), src, m_newId, m_oldId, ctx);

  src.replaceSmallView(m_oldSmall);
  src.replaceFullView(m_oldFull);
}

void MoveProcess::redo(const score::DocumentContext& ctx) const
{
  auto& tgt = m_tgt.find(ctx);
  moveProcess(m_src.find(ctx), tgt, m_oldId, m_newId, ctx);

  {
    auto h = score::AppContext()
                 .settings<Scenario::Settings::Model>()
                 .getSlotHeight();
    tgt.addSlot(Slot{{m_newId}, m_newId, h});
    tgt.setSmallViewVisible(true);
  }
}

void MoveProcess::serializeImpl(DataStreamInput& s) const
{
  s << m_src << m_tgt << m_oldId << m_newId << m_oldSmall << m_oldFull << m_oldPos << m_addedSlot;
}

void MoveProcess::deserializeImpl(DataStreamOutput& s)
{
  s >> m_src >> m_tgt >> m_oldId >> m_newId >> m_oldSmall >> m_oldFull >> m_oldPos >> m_addedSlot;
}
}