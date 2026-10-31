#pragma once
#ifndef STAGE_MANAGER_HPP
#define STAGE_MANAGER_HPP

class ActorManager;

class StageManager {
public:
  explicit StageManager(ActorManager *actorManager);
  ~StageManager();

  void draw() const;

private:
  ActorManager *actorManager_;
};

#endif // !STAGE_MANAGER_HPP
