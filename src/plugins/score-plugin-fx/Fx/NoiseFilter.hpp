#pragma once
#include "1efilter.hpp"
#include <Engine/Node/Process.hpp>

struct NoiseFilter
{
    ossia::value filter(const ossia::value& val)
    {
      struct vis
      {
          ossia::value operator()() const { return {}; }
          ossia::value operator()(const ossia::impulse&) const { return {}; }

          ossia::value operator()(int i) const
          {
            return nFilt->one_euro_i(i, 1);
          }
          ossia::value operator()(float f) const
          {
            return nFilt->one_euro_f(f, 1);
          }
          ossia::value operator()(bool b) const
          {
            return b;
          }

          ossia::value operator()(const std::string& s) const
          {
            return s;
          }

          ossia::value operator()(const ossia::vec2f& t) const
          {
            return t;
          }

          ossia::value operator()(const ossia::vec3f& t) const
          {
            return t;
          }

          ossia::value operator()(const ossia::vec4f& t) const
          {
            return t;
          }

          ossia::value operator()(const std::vector<ossia::value>& t) const
          {
            std::vector<ossia::value> res;

            for (auto& v : t)
            {
              res.push_back(ossia::apply(*this, v.v));
            }

            return res;
          }

          NoiseFilter* nFilt;

          vis(NoiseFilter* enclosing)
            : nFilt{enclosing}
          {}
      };

      try {
        return ossia::apply(vis{this}, val.v);
      } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
      } catch(...) {
        std::cerr << "error" << std::endl;
      }

      return val;
    }

    one_euro_filter<int> one_euro_i;
    one_euro_filter<float> one_euro_f;
    one_euro_filter<ossia::vec2f> one_euro_v2;
    one_euro_filter<ossia::vec3f> one_euro_v3;
    one_euro_filter<ossia::vec4f> one_euro_v4;
    one_euro_filter<std::vector<ossia::value>> one_euro_v;

    NoiseFilter(const float frequency = 120,
                const float minCutoff = 1,
                const float beta = 1,
                const float dCutoff = 1)
      : one_euro_i{one_euro_filter<int>(frequency, minCutoff, beta, dCutoff)},
        one_euro_f{one_euro_filter<float>(frequency, minCutoff, beta, dCutoff)},
        one_euro_v2{one_euro_filter<ossia::vec2f>(frequency,
                                                  ossia::vec2f{minCutoff},
                                                  ossia::vec2f{beta},
                                                  ossia::vec2f{dCutoff})},
        one_euro_v3{one_euro_filter<ossia::vec3f>(frequency,
                                                  ossia::vec3f{minCutoff},
                                                  ossia::vec3f{beta},
                                                  ossia::vec3f{dCutoff})},
        one_euro_v4{one_euro_filter<ossia::vec4f>(frequency,
                                                  ossia::vec4f{minCutoff},
                                                  ossia::vec4f{beta},
                                                  ossia::vec4f{dCutoff})},
        one_euro_v{one_euro_filter<std::vector<ossia::value>>(frequency,
                                                              std::vector<ossia::value>{minCutoff},
                                                              std::vector<ossia::value>{beta},
                                                              std::vector<ossia::value>{dCutoff})}
    {}
};
