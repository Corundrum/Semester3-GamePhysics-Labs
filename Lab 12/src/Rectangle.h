#pragma once
#ifndef __RECTANGLE__
#define __RECTANGLE__

#include <glm/vec4.hpp>
#include "CollisionShape.h"

class Rectangle final : public CollisionAABB
{
public:
	Rectangle();
	Rectangle(glm::vec4 colour, float width, float height, Material material);
	~Rectangle() override;

	// Inherited via GameObject
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

private:
	glm::vec4 colour;
};


#endif /* defined (__RECTANGLE__) */