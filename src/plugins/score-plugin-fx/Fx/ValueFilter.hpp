#pragma once
#include <Engine/Node/PdNode.hpp>
#include "NoiseFilter/NoiseFilter.hpp"

namespace Nodes::ValueFilter
{
struct Node
{
  struct Metadata : Control::Meta_base
  {
    static const constexpr auto prettyName = "Value filter";
    static const constexpr auto objectKey = "ValueFilter";
    static const constexpr auto category = "Mappings";
    static const constexpr auto author = "ossia score";
    static const constexpr auto tags = std::array<const char*, 0>{};
    static const constexpr auto kind = Process::ProcessCategory::Mapping;
    static const constexpr auto description = "Filter and limit noisy value stream";
    static const uuid_constexpr auto uuid = make_uuid("809c014d-7d02-45dc-8849-de7a7db5fe67");

    static const constexpr auto controls = std::make_tuple(
          Control::make_enum(
            "Type",
            0U,
            ossia::make_array(
              "1Euro",
              "Exponential"
              "Average",
              "None")),
          Control::FloatSlider{"amount", 0., 1., .5},
          Control::Toggle{"rate", false},
          Control::IntSpinBox{"ms.", 0, 100, 0},
          Control::Widgets::QuantificationChooser());

    static const constexpr value_in value_ins[]{"in"};
    static const constexpr value_out value_outs[]{"out"};
  };

  struct State
  {
      NoiseFilter nf{};

      ossia::value smooth(const ossia::value& value, const std::string& smoothType, const float& amount)
      {
        if (smoothType == "1Euro")
          return nf.filter(value);
        else if (smoothType == "Average");
      }
  };

  using control_policy = ossia::safe_nodes::last_tick;

  static void
  run(const ossia::value_port& in,
      const std::string& type,
      float amount,
      bool rate,
      int ms,
      float quantif,
      ossia::value_port& out,
      ossia::token_request t,
      ossia::exec_state_facade st,
      State& self)
  {
    for (const ossia::timed_value& v : in.get_data())
    { 
      if (rate)
      {
      }
      else
      {
        out.write_value(self.smooth(v.value, type, amount), v.timestamp);
      }
    }
  }

  static void item(
      Process::Enum& type,
      Process::FloatSlider& amount,
      Process::Toggle& rate,
      Process::IntSpinBox& ms,
      Process::ComboBox& quantif,
      const Process::ProcessModel& process,
      QGraphicsItem& parent,
      QObject& context,
      const Process::Context& doc)
  {
    using namespace Process;
    const Process::PortFactoryList& portFactory = doc.app.interfaces<Process::PortFactoryList>();
    const auto tMarg = 10;
    const auto cMarg = 15;
    const auto h = 85;
    const auto w = 220;

    auto c0_bg = new score::BackgroundItem{&parent};
    c0_bg->setRect({0., 0., w, h});
    auto c1_bg = new score::BackgroundItem{&parent};
    c1_bg->setRect({0., h, w, 45.});

    auto type_item = makeControl(
          std::get<0>(Metadata::controls), type, parent, context, doc, portFactory);
    type_item.root.setPos(tMarg, 0);
    type_item.control.rows = 1;
    type_item.control.columns = 3;
    type_item.control.setRect(QRectF{0, 0, 200, 25});

    auto amount_item = makeControl(
        std::get<1>(Metadata::controls), amount, parent, context, doc, portFactory);
    amount_item.root.setPos(tMarg, 40);
    amount_item.control.setPos(0, cMarg);

    auto rate_item = makeControl(
          std::get<2>(Metadata::controls), rate, parent, context, doc, portFactory);
    rate_item.root.setPos(tMarg, h);
    rate_item.control.setPos(0, tMarg);

    auto ms_item = makeControl(
          std::get<3>(Metadata::controls), ms, parent, context, doc, portFactory);
    ms_item.root.setPos(55, h);
    ms_item.control.setPos(0, cMarg);

    auto quant_item = makeControl(
          std::get<4>(Metadata::controls), quantif, parent, context, doc, portFactory);
    quant_item.root.setPos(130, h);
    quant_item.control.setPos(cMarg, cMarg);
  }
};
}
