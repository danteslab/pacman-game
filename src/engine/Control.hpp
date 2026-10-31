#pragma once
#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <list>

#include "ControllableObject.hpp"
#include "Peripheral.hpp"

class Control {
public:
  typedef struct {
    ControllableObject::action_t action;
    Peripheral *peripheral;
    Peripheral::component_t component;
    Peripheral::event_t oldEvent;
    Peripheral::event_t event;
  } binding_t;

  Control();
  ~Control();

  void addActionPeripheral(ControllableObject::action_t action, Peripheral *peripheral,
                           Peripheral::component_t component, Peripheral::event_t event);
  void setOwner(ControllableObject *owner);
  ControllableObject *getOwner() const;
  void update();
  void reset();

private:
  ControllableObject *owner_;
  std::list<binding_t> bindings_;
};

#endif // !CONTROL_HPP
