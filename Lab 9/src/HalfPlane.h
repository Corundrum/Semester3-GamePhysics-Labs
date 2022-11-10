#pragma once
#include "DisplayObject.h"

class HalfPlane : public DisplayObject
{
public:
	HalfPlane();

	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

	void SetNormalAngle(float angle);
	void SetPlaneAngle();
	float GetNormalAngle();
	float GetPlaneAngle();
	glm::vec2 GetPlaneVector();
	glm::vec2 GetNormalVector();

private:
	glm::vec2 m_normalVector = glm::vec2(0, -1);
	glm::vec2 m_planeVector;
	float m_planeAngle;
	float m_normalAngle = 90.0f;
	const float NORMAL_RENDER_SCALE = 0.0f;

};

