#pragma once
#ifndef __PHYSICS_MANAGER__
#define __PHYSICS_MANAGER__

#include "SDL.h"
#include <vector>
#include "glm/vec2.hpp"
#include "imgui.h"
#include "PhysicsObject.h"
#include <optional>

class PhysicsManager
{
public:
	static PhysicsManager& Instance()
	{
		static PhysicsManager instance;
		return instance;
	}

	void RunPhysics();

	void addPhysicsObject(PhysicsObject* object);
	void deletePhysicsObjects();

	float getGravity();
	float getDrag();

	void setGravity(float gravity);
	void setDrag(float drag);

private:
	PhysicsManager();
	~PhysicsManager();

	/*------ PRIVATE MEMBER FUNCTIONS -------*/

	/*------ PRIVATE MEMBER VARIABLES ------*/

	std::vector<PhysicsObject*> m_physicsList;
	float m_gravity = 9.8f;
	float m_drag = -0.3f;

};

#endif __PHYSICS_MANAGER__ // !__PHYSICS_MANAGER__