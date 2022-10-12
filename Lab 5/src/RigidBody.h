#pragma once
#ifndef __RIGID_BODY__
#define __RIGID_BODY__
#include <glm/vec2.hpp>
class GameObject;

struct RigidBody
{
	GameObject* gameObject = nullptr;
	float mass = 1.0f;
	glm::vec2 velocity = glm::vec2(0, 0);
	float gravityScale = 1.0f;
	float damping = 0.98f;
	bool isColliding = false;
	float radius = 30.0f;
};
#endif /* defined (__RIGID_BODY__) */