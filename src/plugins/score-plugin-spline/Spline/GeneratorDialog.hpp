#pragma once
#include <Process/Script/ScriptEditor.hpp>

#include <QDialog>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>

#include <Spline/Commands.hpp>
#include <exprtk.hpp>

namespace Spline
{
class GeneratorDialog : public Process::ScriptDialog
{
public:
  GeneratorDialog(const ProcessModel& model, const score::DocumentContext& ctx, QWidget* parent)
      : Process::ScriptDialog{"exprtk", ctx, parent}, m_model{model}
  {
    auto step = new QDoubleSpinBox{this};
    step->setRange(0.0001, 0.3);
    step->setValue(m_step);
    step->setSingleStep(0.001);
    step->setDecimals(8);
    // step->setStepType(QSpinBox::AdaptiveDecimalStepType);
    auto lay = static_cast<QBoxLayout*>(this->layout());
    auto controls = new QFormLayout;
    controls->addRow("Step (smaller is more precise)", step);
    lay->insertLayout(2, controls);
    connect(step, qOverload<double>(&QDoubleSpinBox::valueChanged), this, [=](double step) {
      m_step = step;
    });

    syms.add_variable("t", t);
    syms.add_variable("x", x);
    syms.add_variable("y", y);
    syms.add_constants();
    expr.register_symbol_table(syms);

    setText(R"_(x := cos(2 * PI * t);
y := sin(2 * PI * t);
)_");
  }

  void on_accepted() override
  {
    this->setError(0, QString{});
    auto txt = this->text().toStdString();
    bool ok = parser.compile(txt, this->expr);
    if (!ok)
    {
      setError(0, QString::fromStdString(parser.error()));
      return;
    }
    else
    {
      ossia::nodes::spline_data data;
      for (t = 0.; t < 1.; t += m_step)
      {
        expr.value();
        data.points.push_back({x, y});
      }
      {
        t = 1.;
        expr.value();
        data.points.push_back({x, y});
      }

      CommandDispatcher<>{m_context.commandStack}.submit<ChangeSpline>(m_model, std::move(data));
    }
  }
  double t{}, x{}, y{};
  double m_step{0.03};
  exprtk::symbol_table<double> syms;
  exprtk::expression<double> expr;
  exprtk::parser<double> parser;

  const ProcessModel& m_model;
};

}
