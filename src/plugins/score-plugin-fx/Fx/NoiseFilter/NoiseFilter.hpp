#ifndef NOISEFILTER_H
#define NOISEFILTER_H

#pragma once
#include "1efilter.hpp"
#include <Engine/Node/Process.hpp>

class NoiseFilter
{
  public:
    NoiseFilter(float frequency = 120,
    float mincutoff = 1,
    float beta = 1,
    float dcutoff = 1)
      : one_euro_i{frequency, (int) mincutoff, (int) beta, (int) dcutoff},
        one_euro_f{frequency, mincutoff, beta, dcutoff},
        one_euro_i{frequency, (int) mincutoff, (int) beta, (int) dcutoff},
        one_euro_i{frequency, (int) mincutoff, (int) beta, (int) dcutoff},
        one_euro_i{frequency, (int) mincutoff, (int) beta, (int) dcutoff},
        one_euro_i{frequency, (int) mincutoff, (int) beta, (int) dcutoff}{}

    float f;
    float m;
    float b;
    float d;

    ossia::value filter(const ossia::value& val)
    {
      struct vis
      {
          ossia::value operator()() const { return {}; }
          ossia::value operator()(const ossia::impulse&) const { return {}; }

          ossia::value operator()(int i) const
          {
            return one_euro_i(i,1);
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
    one_euro_filter<int> one_euro_i;

//    static one_euro_filter<float> one_euro_f{frequency, mincutoff,
//          beta, dcutoff},
//    static one_euro_filter<ossia::vec2f> one_euro_v2{
//      frequency, ossia::vec2f{mincutoff},
//      ossia::vec2f{beta},
//      ossia::vec2f{dcutoff}},
//    static one_euro_filter<ossia::vec3f> one_euro_v3{
//      frequency, ossia::vec3f{mincutoff},
//      ossia::vec3f{beta},
//      ossia::vec3f{dcutoff}},
//    static one_euro_filter<ossia::vec4f> one_euro_v4{
//      frequency, ossia::vec4f{mincutoff},
//      ossia::vec4f{beta},
//      ossia::vec4f{dcutoff}},
//    static one_euro_filter<std::vector<ossia::value>> one_euro_v{
//      frequency, std::vector<ossia::value>{mincutoff},
//      std::vector<ossia::value>{beta},
//      std::vector<ossia::value>{dcutoff}}
};

#endif // NOISEFILTER_H
