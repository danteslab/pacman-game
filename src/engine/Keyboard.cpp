#include "Keyboard.hpp"

Keyboard::Keyboard() {}

Peripheral::state_t Keyboard::getState(component_t component) {
  return IsKeyDown(component) ? 1 : INVALID_STATE;
}

Keyboard::~Keyboard() {}
