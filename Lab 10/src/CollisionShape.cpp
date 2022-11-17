#include "CollisionShape.h"
#include "Util.h"

/*---------------CIRLCE---------------*/
Shape CollisionCircle::GetShape()
{
	return CIRCLE;
}

float CollisionCircle::GetRadius()
{
	return radius;
}

void CollisionCircle::SetRadius(float radius)
{
	this->radius = radius;
}


/*---------------HALFPLANE---------------*/
Shape CollisionHalfPlane::GetShape()
{
	return HALFPLANE;
}

void CollisionHalfPlane::SetNormalAngle(float angle)
{
	m_normalAngle = angle;
	m_normalVector = Util::AngleMagnitudeToVec2(m_normalAngle, 1);
	SetPlaneAngle();
}

void CollisionHalfPlane::SetPlaneAngle()
{
	m_planeAngle = m_normalAngle + 90;
	m_planeVector = Util::AngleMagnitudeToVec2(m_planeAngle, 1.0f);
}

float CollisionHalfPlane::GetNormalAngle()
{
	return m_normalAngle;
}

float CollisionHalfPlane::GetPlaneAngle()
{
	return m_planeAngle;
}

glm::vec2 CollisionHalfPlane::GetPlaneVector()
{
	return m_planeVector;
}

glm::vec2 CollisionHalfPlane::GetNormalVector()
{
	return m_normalVector;
}
