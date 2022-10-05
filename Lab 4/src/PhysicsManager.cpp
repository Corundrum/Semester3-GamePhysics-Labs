#include "PhysicsManager.h"

#include <algorithm>

PhysicsManager::PhysicsManager()
= default;

PhysicsManager::~PhysicsManager()
{
	deletePhysicsObjects();
}

void PhysicsManager::RunPhysics()
{
	for (auto& physics_object : m_physicsList)
	{
		if (physics_object != nullptr)
		{
			if (!physics_object->IsEnabled())
			{
				break;
			}

			//gravity
			physics_object->GetRigidBody()->velocity.y += getGravity() * physics_object->getGravityEffect();
			//drag
			physics_object->GetRigidBody()->acceleration = ((physics_object->GetRigidBody()->velocity * physics_object->GetRigidBody()->velocity) * getDrag() * physics_object->getDampenEffect()) * (1.0f/60.0f);
			//velocity
			physics_object->GetRigidBody()->velocity += physics_object->GetRigidBody()->acceleration * (1.0f / 60.0f);
			//moving
			physics_object->GetTransform()->position += physics_object->GetRigidBody()->velocity * (1.0f / 60.0f);

		}
	}
}

void PhysicsManager::addPhysicsObject(PhysicsObject* object)
{
	m_physicsList.push_back(object);
}

void PhysicsManager::deletePhysicsObjects()
{
	for (auto& object : m_physicsList)
	{
		delete object;
		object = nullptr;
	}
	m_physicsList.clear();
}

float PhysicsManager::getGravity()
{
	return m_gravity;
}

float PhysicsManager::getDrag()
{
	return m_drag;
}

void PhysicsManager::setGravity(float gravity)
{
	m_gravity = gravity;
}

void PhysicsManager::setDrag(float drag)
{
	m_drag = drag;
}
