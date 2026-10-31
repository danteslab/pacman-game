#include "Control.hpp"

Control::Control() : owner_(nullptr) {}

Control::~Control() {}

void Control::addActionPeripheral(ControllableObject::action_t action, Peripheral *peripheral,
                                  Peripheral::component_t component, Peripheral::event_t event) {
  binding_t binding;
  binding.action = action;
  binding.peripheral = peripheral;
  binding.component = component;
  binding.oldEvent = Peripheral::NO_EVENT;
  binding.event = event;
  bindings_.push_back(binding);
}

void Control::setOwner(ControllableObject *owner) { owner_ = owner; }

ControllableObject *Control::getOwner() const { return owner_; }

void Control::update() {
  for (auto &binding : bindings_) {
    Peripheral::state_t currentState = binding.peripheral->getState(binding.component);
    Peripheral::event_t previousEvent = binding.oldEvent;
    bool trigger = false;

    switch (binding.event) {
      case Peripheral::ON_PRESSING:
        if (currentState != INVALID_STATE) {
          trigger = true;
        }
        break;
      case Peripheral::ON_PRESS:
        if (previousEvent == Peripheral::ON_RELEASE && currentState != INVALID_STATE) {
          binding.oldEvent = Peripheral::ON_PRESS;
          trigger = true;
        } else if (currentState == INVALID_STATE) {
          binding.oldEvent = Peripheral::ON_RELEASE;
        }
        break;
      case Peripheral::ON_RELEASE:
        if (previousEvent == Peripheral::ON_PRESS && currentState == INVALID_STATE) {
          binding.oldEvent = Peripheral::ON_RELEASE;
          trigger = true;
        } else if (currentState != INVALID_STATE) {
          binding.oldEvent = Peripheral::ON_PRESS;
        }
        break;
      case Peripheral::ON_RELEASING:
        if (currentState == INVALID_STATE) {
          trigger = true;
        }
        break;
      default:
        break;
    }

    if (trigger && owner_ != nullptr) {
      owner_->doAction(binding.action, currentState);
    }
  }
}

void Control::reset() {
  for (auto &binding : bindings_) {
    binding.oldEvent = Peripheral::NO_EVENT;
  }
}
