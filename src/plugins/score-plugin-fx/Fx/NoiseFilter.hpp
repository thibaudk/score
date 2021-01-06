#pragma once
#include <Engine/Node/Process.hpp>
#include <dno/src/DeNoiser.hpp>

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
            return nFilt->dno_i(i);
          }
          ossia::value operator()(float f) const
          {
            return nFilt->dno_f(f);
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

    DeNoiser<int> dno_i;
    DeNoiser<float> dno_f;
    DeNoiser<ossia::vec2f> dno_v2;
    DeNoiser<ossia::vec3f> dno_v3;
    DeNoiser<ossia::vec4f> dno_v4;
    DeNoiser<std::vector<ossia::value>> dno_v;

    NoiseFilter(const float frequency = 120,
                const float minCutoff = 1,
                const float beta = 1,
                const float dCutoff = 1)
      : dno_i{DeNoiser<int>(frequency, minCutoff, beta, dCutoff)},
        dno_f{DeNoiser<float>(frequency, minCutoff, beta, dCutoff)},
        dno_v2{DeNoiser<ossia::vec2f>(frequency,
                                                  ossia::vec2f{minCutoff},
                                                  ossia::vec2f{beta},
                                                  ossia::vec2f{dCutoff})},
        dno_v3{DeNoiser<ossia::vec3f>(frequency,
                                                  ossia::vec3f{minCutoff},
                                                  ossia::vec3f{beta},
                                                  ossia::vec3f{dCutoff})},
        dno_v4{DeNoiser<ossia::vec4f>(frequency,
                                                  ossia::vec4f{minCutoff},
                                                  ossia::vec4f{beta},
                                                  ossia::vec4f{dCutoff})},
        dno_v{DeNoiser<std::vector<ossia::value>>(frequency,
                                                              std::vector<ossia::value>{minCutoff},
                                                              std::vector<ossia::value>{beta},
                                                              std::vector<ossia::value>{dCutoff})}
    {}
};
