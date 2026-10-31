#pragma once
#ifndef PERIPHERAL_HPP
#define PERIPHERAL_HPP

#define INVALID_STATE -2
#define INVALID_COMPONENT -3

class Peripheral {
public:
  typedef enum { NO_EVENT = 0, ON_PRESS, ON_PRESSING, ON_RELEASE, ON_RELEASING } event_t;

  typedef int state_t;
  typedef int component_t;

  virtual ~Peripheral() = default;
  virtual state_t getState(component_t component) = 0;
};

#endif // !PERIPHERAL_HPP
