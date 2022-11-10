#include "HalfPlane.h"
#include "Util.h"

HalfPlane::HalfPlane()
{
	SetType(GameObjectType::HALFPLANE);
	GetRigidBody()->gravityScale = 0.0f;
	SetNormalAngle(m_normalAngle);
}

void HalfPlane::Draw()
{
	//draw normal line
	glm::vec2 start = GetTransform()->position;
	glm::vec2 end = start + (m_normalVector * NORMAL_RENDER_SCALE);
	Util::DrawLine(start, end);

	//draw plane line
	Util::DrawLine(start - (m_planeVector * 10000.0f), start + (m_planeVector * 10000.0f), glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));
}

void HalfPlane::Update()
{
}

void HalfPlane::Clean()
{
}

void HalfPlane::SetNormalAngle(float angle)
{
	m_normalAngle = angle;
	m_normalVector = Util::AngleMagnitudeToVec2(m_normalAngle, 1);
	SetPlaneAngle();
}

void HalfPlane::SetPlaneAngle()
{
	m_planeAngle = m_normalAngle + 90;
	m_planeVector = Util::AngleMagnitudeToVec2(m_planeAngle, 1.0f);
}

float HalfPlane::GetNormalAngle()
{
	return m_normalAngle;
}

float HalfPlane::GetPlaneAngle()
{
	return m_planeAngle;
}

glm::vec2 HalfPlane::GetPlaneVector()
{
	return m_planeVector;
}

glm::vec2 HalfPlane::GetNormalVector()
{
	return m_normalVector;
}
