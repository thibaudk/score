#include "AddRackToConstraint.hpp"

#include "Document/Constraint/ConstraintModel.hpp"
#include "Document/Constraint/Rack/RackModel.hpp"
#include "Document/Constraint/ViewModels/FullView/FullViewConstraintViewModel.hpp"
#include <iscore/tools/SettableIdentifierGeneration.hpp>

using namespace iscore;
using namespace Scenario::Command;

AddRackToConstraint::AddRackToConstraint(Path<ConstraintModel>&& constraintPath) :
    SerializableCommand {"ScenarioControl",
                         commandName(),
                         description()},
    m_path {constraintPath}
{
    auto& constraint = m_path.find();
    m_createdRackId = getStrongId(constraint.racks);
}

void AddRackToConstraint::undo() const
{
    auto& constraint = m_path.find();
    constraint.racks.remove(m_createdRackId);
}

void AddRackToConstraint::redo() const
{
    auto& constraint = m_path.find();
    auto rack = new RackModel{m_createdRackId, &constraint};

    constraint.racks.add(rack);

    // If it is the first rack created,
    // it is also assigned to the full view of the constraint.
    if(constraint.racks.size() == 1)
    {
        constraint.fullView()->showRack(m_createdRackId);
    }
}

void AddRackToConstraint::serializeImpl(QDataStream& s) const
{
    s << m_path << m_createdRackId;
}

void AddRackToConstraint::deserializeImpl(QDataStream& s)
{
    s >> m_path >> m_createdRackId;
}
