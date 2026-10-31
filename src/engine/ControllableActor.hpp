#pragma once
#ifndef CONTROLLABLE_ACTOR_HPP
#define CONTROLLABLE_ACTOR_HPP

#include "Actor.hpp"
#include "ControllableObject.hpp"

class ControllableActor : public Actor, public ControllableObject {};

#endif // !CONTROLLABLE_ACTOR_HPP
