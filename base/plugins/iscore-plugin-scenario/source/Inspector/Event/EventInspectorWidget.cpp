#include "EventInspectorWidget.hpp"

#include "Document/Event/EventModel.hpp"
#include "Commands/Event/AddStateToEvent.hpp"
#include "Commands/Event/SetCondition.hpp"
#include "Commands/Event/SetTrigger.hpp"

#include "Document/TimeNode/TimeNodeModel.hpp"

#include <Inspector/InspectorSectionWidget.hpp>
#include "Inspector/MetadataWidget.hpp"

#include "base/plugins/iscore-plugin-deviceexplorer/Plugin/Panel/DeviceExplorerModel.hpp"

#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QFormLayout>
#include <QCompleter>

#include <Process/ScenarioModel.hpp>
#include <DeviceExplorer/../Plugin/Widgets/DeviceCompleter.hpp>
#include <DeviceExplorer/../Plugin/Widgets/DeviceExplorerMenuButton.hpp>
#include <Singletons/DeviceExplorerInterface.hpp>
#include "Document/Constraint/ConstraintModel.hpp"

#include <iscore/document/DocumentInterface.hpp>
#include <core/document/Document.hpp>

#include "Inspector/Separator.hpp"
#include "Inspector/SelectionButton.hpp"
#include "core/document/DocumentModel.hpp"
#include "Inspector/State/StateInspectorWidget.hpp"
#include <Inspector/InspectorWidgetList.hpp>
#include <iscore/widgets/MarginLess.hpp>

EventInspectorWidget::EventInspectorWidget(
        const EventModel& object,
        iscore::Document& doc,
        QWidget* parent) :
    InspectorWidgetBase {object, doc, parent},
    m_model {object}
{
    setObjectName("EventInspectorWidget");
    setParent(parent);

    con(m_model, &EventModel::statesChanged,
            this,    &EventInspectorWidget::updateDisplayedValues);
    con(m_model, &EventModel::dateChanged,
            this,   &EventInspectorWidget::modelDateChanged);

    ////// HEADER
    // metadata
    m_metadata = new MetadataWidget{&m_model.metadata, commandDispatcher(), &m_model, this};
    m_metadata->setType(EventModel::prettyName());
    m_metadata->setupConnections(m_model);

    addHeader(m_metadata);

    ////// BODY
    /// Information
    auto infoWidg = new QWidget;
    auto infoLay = new iscore::MarginLess<QFormLayout>;
    infoWidg->setLayout(infoLay);

    // timeNode
    auto timeNode = m_model.timeNode();
    if(timeNode)
    {
        auto scenar = m_model.parentScenario();
        auto tnBtn = SelectionButton::make(
                    tr("Parent TimeNode"),
                    &scenar->timeNode(timeNode),
                    selectionDispatcher(),
                    this);

        infoLay->addWidget(tnBtn);
    }

    // date
    auto datewidg = new QWidget;
    auto dateLay = new iscore::MarginLess<QHBoxLayout>;
    datewidg->setLayout(dateLay);
    m_date = new QLabel{QString::number(m_model.date().msec())};

    dateLay->addWidget(new QLabel(tr("Default date")));
    dateLay->addWidget(m_date);

    infoLay->addWidget(datewidg);
    m_properties.push_back(infoWidg);


    // Separator
    m_properties.push_back(new Separator {this});

    /// Condition
    m_conditionLineEdit = new QLineEdit{this};
    m_conditionLineEdit->setValidator(&m_validator);

    connect(m_conditionLineEdit, &QLineEdit::editingFinished,
            this, &EventInspectorWidget::on_conditionChanged);
    con(m_model, &EventModel::conditionChanged,
        this, [this] (const iscore::Condition& c) {
        m_conditionLineEdit->setText(c.toString());
    });

    m_properties.push_back(new QLabel{tr("Condition")});
    m_properties.push_back(m_conditionLineEdit);

    // State
    m_properties.push_back(new Separator {this});
    m_statesWidget = new QWidget{this};
    auto dispLayout = new QVBoxLayout{m_statesWidget};
    m_statesWidget->setLayout(dispLayout);

    m_properties.push_back(new QLabel{"States"});
    m_properties.push_back(m_statesWidget);

    // Separator
    m_properties.push_back(new Separator {this});

    // Plugins (TODO factorize with ConstraintInspectorWidget)
    for(auto& plugdata : m_model.pluginModelList.list())
    {
        for(iscore::DocumentDelegatePluginModel* plugin : doc.model().pluginModels())
        {
            auto md = plugin->makeElementPluginWidget(plugdata, this);
            if(md)
            {
                m_properties.push_back(md);
                break;
            }
        }
    }

    updateDisplayedValues();


    // Display data
    updateAreaLayout(m_properties);
}

void EventInspectorWidget::addState(const StateModel& state)
{
    auto sw = new StateInspectorWidget{state, doc(), this};

    m_states.push_back(sw);
    m_statesWidget->layout()->addWidget(sw);
}

void EventInspectorWidget::removeState(const StateModel& state)
{
    // this is not connected
    ISCORE_TODO;
}

void EventInspectorWidget::focusState(const StateModel* state)
{
    ISCORE_TODO;
}

void EventInspectorWidget::updateDisplayedValues()
{
    // Cleanup
    for(auto& elt : m_states)
    {
        delete elt;
    }

    m_states.clear();
    m_date->clear();

    m_date->setText(QString::number(m_model.date().msec()));

    const auto& scenar = m_model.parentScenario();
    for(const auto& state : m_model.states())
    {
        addState(scenar->state(state));
    }

    m_conditionLineEdit->setText(m_model.condition().toString());
}


QVariant textToVariant(const QString& txt)
{
    bool ok = false;
    if(float val = txt.toFloat(&ok))
    {
        return val;
    }
    if(int val = txt.toInt(&ok))
    {
        return val;
    }

    return txt;
}

QVariant textToMessageValue(const QStringList& txt)
{
    if(txt.empty())
    {
        return {};
    }
    else if(txt.size() == 1)
    {
        return textToVariant(txt.first());
    }
    else
    {
        QVariantList vl;
        for(auto& elt : txt)
        {
            vl.append(textToVariant(elt));
        }
        return vl;
    }
}
using namespace iscore::IDocument;
using namespace Scenario;

void EventInspectorWidget::on_conditionChanged()
{
    auto cond = m_validator.get();

    if(*cond != m_model.condition())
    {
        auto cmd = new Scenario::Command::SetCondition{path(m_model), std::move(*cond)};
        emit commandDispatcher()->submitCommand(cmd);
    }
}

void EventInspectorWidget::modelDateChanged()
{
    m_date->setText(QString::number(m_model.date().msec()));
}
