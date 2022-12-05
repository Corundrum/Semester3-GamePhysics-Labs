#pragma once
#ifndef __BOX__
#define __BOX__

#include <glm/vec4.hpp>
#include "CollisionShape.h"

class Box final : public CollisionAABB
{
public:
	Box();
	Box(glm::vec4 colour, float width, float height, Material material);
	~Box() override;

	// Inherited via GameObject
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

private:
	glm::vec4 colour;
};


#endif /* defined (__BOX__) */