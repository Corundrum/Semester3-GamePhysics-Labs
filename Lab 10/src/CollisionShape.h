#pragma once
#include "DisplayObject.h"
/*---------------SHAPE NAMES---------------*/
enum Shape
{
	CIRCLE,
	HALFPLANE
};

/*---------------DEFAULT SHAPE---------------*/
class CollisionShape : public DisplayObject
{
public:
	virtual void Update() override = 0;
	virtual void Draw() override = 0;
	virtual void Clean() override = 0;

	virtual Shape GetShape() = 0;
};

/*---------------SHAPES---------------*/
class CollisionCircle : public CollisionShape
{
public:
	virtual void Update() override = 0;
	virtual void Draw() override = 0;
	virtual void Clean() override = 0;

	virtual Shape GetShape() override;

	float GetRadius();
	void SetRadius(float radius);

	

private:
	float radius = 30.0f;
};

class CollisionHalfPlane : public CollisionShape
{
public:
	virtual void Update() override = 0;
	virtual void Draw() override = 0;
	virtual void Clean() override = 0;

	virtual Shape GetShape() override;

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
};