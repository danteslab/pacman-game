#pragma once
#ifndef CONTROL_MANAGER_HPP
#define CONTROL_MANAGER_HPP

#include <vector>

class Control;
class Peripheral;

class ControlManager {
public:
  ControlManager();
  ~ControlManager();

  int addControl(Control *control);
  int addPeripheral(Peripheral *peripheral);
  void update();

private:
  std::vector<Control *> controls_;
  std::vector<Peripheral *> peripherals_;
};

#endif // !CONTROL_MANAGER_HPP
