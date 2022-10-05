#include "PhysicsObject.h"
#include "PhysicsManager.h"
#include "Game.h"

PhysicsObject::PhysicsObject() : m_gravityEffect(1.0f), m_dampenEffect(1.0f)
{
	PhysicsManager::Instance().addPhysicsObject(this);
}

PhysicsObject::~PhysicsObject()
= default;

void PhysicsObject::Update()
{
	//gravity
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
