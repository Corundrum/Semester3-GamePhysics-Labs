#include "HalfPlane.h"
#include "Util.h"

void HalfPlane::Draw()
{
	//draw normal
	glm::vec2 start = GetTransform()->position;
	glm::vec2 end = start + (m_normal * NORMAL_RENDER_SCALE);
	Util::DrawLine(start, end);


	glm::vec2 perpendicular = Util::AngleMagnitudeToVec2(m_orientation + 90, 10000.0f);
	Util::DrawLine(start - perpendicular, start + perpendicular, glm::vec4(0.6f, 0.0f, 0.35f, 1.0f));

}

void HalfPlane::Update()
{
}

void HalfPlane::Clean()
{
}

void HalfPlane::SetOrientation(float angle)
{
	m_orientation = angle;
	m_normal = Util::AngleMagnitudeToVec2(m_orientation, 1);
}
