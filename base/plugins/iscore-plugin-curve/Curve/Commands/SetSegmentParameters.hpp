#pragma once
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ModelPath.hpp>

class CurveModel;
class CurveSegmentModel;
using SegmentParameterMap = QMap<Id<CurveSegmentModel>, QPair<double, double>>;
class SetSegmentParameters : public iscore::SerializableCommand
{
        ISCORE_COMMAND_DECL("AutomationControl", "SetSegmentParameters", "SetSegmentParameters")
    public:
        ISCORE_SERIALIZABLE_COMMAND_DEFAULT_CTOR(SetSegmentParameters)
        SetSegmentParameters(Path<CurveModel>&& model, SegmentParameterMap&& parameters);

        void undo() const override;
        void redo() const override;

        void update(Path<CurveModel>&& model, SegmentParameterMap&&  segments);

    protected:
        void serializeImpl(QDataStream & s) const override;
        void deserializeImpl(QDataStream & s) override;

    private:
        Path<CurveModel> m_model;
        SegmentParameterMap m_new;
        QMap<
            Id<CurveSegmentModel>,
            QPair<
                boost::optional<double>,
                boost::optional<double>
            >
        > m_old;
};
