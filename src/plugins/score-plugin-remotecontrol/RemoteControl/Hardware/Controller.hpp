#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

namespace RemoteControl
{

struct Controller
{
  Controller() = default;

  enum Comands
  {
    Play = 0,
    Stop
  };
};

}

#endif // CONTROLLER_HPP
