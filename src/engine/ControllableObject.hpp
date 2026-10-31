#pragma once
#ifndef CONTROLLABLE_OBJECT_HPP
#define CONTROLLABLE_OBJECT_HPP

class ControllableObject {
public:
  typedef short action_t;

  virtual ~ControllableObject() = default;
  virtual void doAction(action_t action, int magnitude) = 0;
};

#endif // !CONTROLLABLE_OBJECT_HPP
