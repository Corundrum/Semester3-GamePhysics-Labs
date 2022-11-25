#include "CollisionShape.h"
#include "Util.h"

/*---------------CIRLCE---------------*/
Shape CollisionCircle::GetShape()
{
	return CIRCLE;
}

int CollisionCircle::GetSize()
{
	return (3.14159 * pow(GetRadius(), 2));
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

int CollisionHalfPlane::GetSize()
{
	return 100000;
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


/*---------------DEFAULT---------------*/
void CollisionShape::SetMaterial(Material type)
{
	switch (type)
	{
	case STEEL:
		this->GetRigidBody()->friction = 0.4f;
		this->GetRigidBody()->mass = this->GetSize() * 1.5f;
		this->GetRigidBody()->restitution = 0.2f;
		break;

	case WOOD:
		this->GetRigidBody()->friction = 0.5f;
		this->GetRigidBody()->mass = this->GetSize() * 0.9f;
		this->GetRigidBody()->restitution = 0.5f;
		break;

	case STONE:
		this->GetRigidBody()->friction = 0.8f;
		this->GetRigidBody()->mass = this->GetSize() * 1.2f;
		this->GetRigidBody()->restitution = 0.25f;
		break;

	case RUBBER:
		this->GetRigidBody()->friction = 0.6f;
		this->GetRigidBody()->mass = this->GetSize() * 0.6f;
		this->GetRigidBody()->restitution = 0.8f;
		break;

	case DIRT:
		this->GetRigidBody()->friction = 0.3f;
		this->GetRigidBody()->mass = this->GetSize() * 1.0f;
		this->GetRigidBody()->restitution = 0.4f;
		break;
	}
}
