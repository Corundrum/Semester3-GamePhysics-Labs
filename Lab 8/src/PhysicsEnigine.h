#pragma once
#include <list>
#include "glm/vec2.hpp"
class RigidBody;
class HalfPlane;

class PhysicsEngine
{
public:
	static PhysicsEngine& Instance()
	{
		static PhysicsEngine instance; // Magic statics.
		return instance;
	}

	float fixedDeltatime = 1.0f / 60.0f;
	glm::vec2 gravityAcceleration = glm::vec2(0, 9.81);

public:
	void AddPhysicsObject(RigidBody* rigidBody);
	void AddPlaneObject(HalfPlane* halfplane);
	void Update();

private:
	std::list<RigidBody*> physicsObject;
	std::list<HalfPlane*> physicsPlanes;

}; 