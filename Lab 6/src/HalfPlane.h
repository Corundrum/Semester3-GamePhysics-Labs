#pragma once
#include "DisplayObject.h"

class HalfPlane : public DisplayObject
{
public:
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

	void SetOrientation(float angle);

private:
	glm::vec2 m_normal = glm::vec2(0, -1);
	float m_orientation = 90.0f;
	const float NORMAL_RENDER_SCALE = 100.0f;

};

