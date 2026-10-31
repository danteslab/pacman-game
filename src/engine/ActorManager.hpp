#pragma once
#ifndef ACTOR_MANAGER_HPP
#define ACTOR_MANAGER_HPP

#include <list>

class Actor;

class ActorManager {
public:
  ActorManager();
  ~ActorManager();

  void add(Actor *actor);
  void del(Actor *actor);
  void update();
  void drawAll() const;
  int size() const;

private:
  std::list<Actor *> actors_;
};

#endif // !ACTOR_MANAGER_HPP
