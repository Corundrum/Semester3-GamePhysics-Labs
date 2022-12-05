#pragma once
#ifndef __RIGID_BODY__
#define __RIGID_BODY__
#include <glm/vec2.hpp>
#include <iostream>

struct RigidBody
{
	std::string name = "Default Body";
	float mass = 1.0f;
	float friction = 1.0f;
	float restitution = 0.8f;
	glm::vec2 netForce = glm::vec2(0.0f, 0.0f);
	glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
	glm::vec2 acceleration = glm::vec2(0.0f, 0.0f);
	float gravityScale = 1.0f;
	bool isColliding = false;
};
#endif /* defined (__RIGID_BODY__) */