#pragma once
#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include <raylib.h>

#include "Peripheral.hpp"

class Keyboard : public Peripheral {
public:
  Keyboard();
  ~Keyboard() override;

  state_t getState(component_t component) override;
};

#endif // !KEYBOARD_HPP
