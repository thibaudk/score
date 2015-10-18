#pragma once
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ModelPath.hpp>

#include <DeviceExplorer/Node/DeviceExplorerNode.hpp>
#include <Document/State/ItemModel/MessageItemModel.hpp>


class RemoveMessageNodes : public iscore::SerializableCommand
{
        ISCORE_COMMAND_DECL("ScenarioControl", "RemoveMessageNodes", "RemoveMessageNodes")
        public:
            ISCORE_SERIALIZABLE_COMMAND_DEFAULT_CTOR(RemoveMessageNodes)

          RemoveMessageNodes(
            Path<MessageItemModel>&& ,
            const iscore::NodeList&);

        void undo() const override;
        void redo() const override;

    protected:
        void serializeImpl(QDataStream&) const override;
        void deserializeImpl(QDataStream&) override;

    private:
        Path<MessageItemModel> m_path;
        QList<iscore::Node> m_savedNodes;
        QList<iscore::NodePath> m_nodePaths;
};
