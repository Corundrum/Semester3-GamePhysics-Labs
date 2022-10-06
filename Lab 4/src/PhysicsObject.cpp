#include "PhysicsObject.h"
#include "PhysicsManager.h"
#include "EventManager.h"
#include "Game.h"
#include "Util.h"

PhysicsObject::PhysicsObject() : m_gravityEffect(1.0f), m_dampenEffect(1.0f)
{
	PhysicsManager::Instance().addPhysicsObject(this);
	GetRigidBody()->velocity = AngleMagnitudeToVec2(getAngle(), getSpeed());
}

PhysicsObject::~PhysicsObject()
= default;

glm::vec2 PhysicsObject::AngleMagnitudeToVec2(float angle, float magnitude)
{
	return glm::vec2
	(
		cos(-angle * Util::Deg2Rad) * magnitude * 5.0f,
		sin(-angle * Util::Deg2Rad) * magnitude* 5.0f
	);
}

void PhysicsObject::Update()
{
	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_SPACE))
	{
		GetTransform()->position = glm::vec2(100.0f, 100.0f);
		GetRigidBody()->acceleration = glm::vec2(100.0f, 100.0f);
		GetRigidBody()->velocity = glm::vec2(100.0f, 100.0f);
		GetRigidBody()->velocity = AngleMagnitudeToVec2(getAngle(), getSpeed());

	}
}

float PhysicsObject::getGravityEffect()
{
	return m_gravityEffect;
}

float PhysicsObject::getDampenEffect()
{
	return m_dampenEffect;
}

float PhysicsObject::getSpeed()
{
	return m_speed;
}

float PhysicsObject::getAngle()
{
	return m_angle;
}

void PhysicsObject::setGravityEffect(float gravity)
{
	m_gravityEffect = gravity;
}

void PhysicsObject::setDampenEffect(float dampen)
{
	m_dampenEffect = dampen;
}

void PhysicsObject::setSpeed(float speed)
{
	m_speed = speed;
}

void PhysicsObject::setAngle(float angle)
{
	m_angle = angle;
}
