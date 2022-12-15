#pragma once
#include <list>
#include "glm/vec2.hpp"
class CollisionShape;
class CollisionCircle;
class CollisionHalfPlane;
class CollisionAABB;

class PhysicsEngine
{
public:
	static PhysicsEngine& Instance()
	{
		static PhysicsEngine instance; // Magic statics.
		return instance;
	}

	float fixedDeltatime = 1.0f / 60.0f;
	glm::vec2 gravityAcceleration = glm::vec2(0.0f, 9.81f);
	
	glm::vec2 FGravity;

	float damping = 0.99f;

public:
	void AddObject(CollisionShape* object);

	bool CircleCircleCheck(CollisionCircle* circle1, CollisionCircle* circle2);
	bool CircleHalfPlaneCheck(CollisionCircle* circle, CollisionHalfPlane* half_plane);
	bool CircleAABBCheck(CollisionCircle* circle, CollisionAABB* aabb);
	bool AABBAABBCheck(CollisionAABB* aabb1, CollisionAABB* aabb2);

	void Update();

private:


	std::list<CollisionShape*> objects;
}; 