#include "Circle.h"
#include "TextureManager.h"
#include "Game.h"

Circle::Circle()
{
	SetType(GameObjectType::CIRCLE);
}

Circle::~Circle()
= default;

void Circle::setRadius(float radius)
{
	m_radius = radius;
}

float Circle::getRadius()
{
	return m_radius;
}
