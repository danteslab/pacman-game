#include "ControlManager.hpp"

#include "Control.hpp"
#include "Peripheral.hpp"

ControlManager::ControlManager() {}

ControlManager::~ControlManager() {
  for (auto *control : controls_) {
    delete control;
  }
}

int ControlManager::addControl(Control *control) {
  controls_.push_back(control);
  return static_cast<int>(controls_.size()) - 1;
}

int ControlManager::addPeripheral(Peripheral *peripheral) {
  peripherals_.push_back(peripheral);
  return static_cast<int>(peripherals_.size()) - 1;
}

void ControlManager::update() {
  for (auto *control : controls_) {
    control->update();
  }
}
