#pragma once
#include <list>
#include "glm/vec2.hpp"
class RigidBody;

class PhysicsEngine
{
public:
	float fixedDeltatime = 1.0f / 60.0f;
	glm::vec2 gravityAcceleration = glm::vec2(0, 981);

public:
	void AddObject(RigidBody* rigidBody);
	void Update();

private:
	std::list<RigidBody*> physicsObject;

}; 