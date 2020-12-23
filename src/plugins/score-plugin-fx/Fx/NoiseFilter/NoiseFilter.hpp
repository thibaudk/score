#ifndef NOISEFILTER_H
#define NOISEFILTER_H

#pragma once
#include "1efilter.hpp"
#include <Engine/Node/Process.hpp>

class NoiseFilter
{
  public:
    NoiseFilter(const double frequency = 120,
    const double mincutoff = 1,
    const double beta = 1,
    const double dcutoff = 1)
      : f{frequency},
        m{mincutoff},
        b{beta},
        d{dcutoff}{}

    ossia::value filter(const ossia::value& val, const float& amt)
    {
      struct vis
      {
          ossia::value operator()() const { return {}; }
          ossia::value operator()(const ossia::impulse&) const { return {}; }

          ossia::value operator()(int i) const
          {
            return i;
          }
          ossia::value operator()(float f) const
          {
            return f;
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
      };

      try {
        return ossia::apply(vis{}, val.v);
      } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
      } catch(...) {
        std::cerr << "error" << std::endl;
      }
      return val;
    }

  private :

    const double f;
    const double m;
    const double b;
    const double d;

    one_euro_filter<> one_euro_f{f, m, b, d};
    one_euro_filter<int> one_euro_i{f, (int) m, (int) b, (int) d};

    one_euro_filter<const ossia::vec2f&> one_euro_v2{
      f, ossia::vec2f{(float) m, (float) m},
      ossia::vec2f{(float) b, (float) b},
      ossia::vec2f{(float) d, (float) d}};

//    one_euro_filter<ossia::vec3f> one_euro_v3{
//      f, ossia::vec3f{(float) m, (float) m, (float) m},
//      ossia::vec3f{(float) b, (float) b, (float) b},
//      ossia::vec3f{(float) d, (float) d, (float) d}};

//    one_euro_filter<ossia::vec4f> one_euro_v4{
//      f, ossia::vec4f{(float) m, (float) m, (float) m, (float) m},
//      ossia::vec4f{(float) b, (float) b, (float) b, (float) b},
//      ossia::vec4f{(float) d, (float) d, (float) d, (float) d}};

    one_euro_filter<std::vector<ossia::value>> one_euro_v{
      f, std::vector<ossia::value>{m},
      std::vector<ossia::value>{b},
      std::vector<ossia::value>{d}};
};

#endif // NOISEFILTER_H
