#pragma once
#include "DisplayObject.h"
/*---------------SHAPE NAMES---------------*/
enum Shape
{
	CIRCLE,
	HALFPLANE
};

enum Material
{
	STEEL,
	WOOD,
	STONE,
	RUBBER,
	DIRT
};

/*---------------DEFAULT SHAPE---------------*/
class CollisionShape : public DisplayObject
{
public:
	virtual void Update() override = 0;
	virtual void Draw() override = 0;
	virtual void Clean() override = 0;

	virtual Shape GetShape() = 0;
	virtual int GetSize() = 0;
	
	virtual void SetMaterial(Material type);
};

/*---------------SHAPES---------------*/
class CollisionCircle : public CollisionShape
{
public:
	virtual void Update() override = 0;
	virtual void Draw() override = 0;
	virtual void Clean() override = 0;

	virtual Shape GetShape() override;
	virtual int GetSize() override;

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
	virtual int GetSize() override;

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