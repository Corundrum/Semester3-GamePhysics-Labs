#pragma once
#ifndef __TARGET__
#define __TARGET__

#include <glm/vec4.hpp>
#include "CollisionShape.h"

class Target final : public CollisionCircle 
{
public:
	Target();
	Target(glm::vec4 colour, float radius, Material material);
	~Target() override;

	// Inherited via GameObject
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

private:
	glm::vec4 colour;
};


#endif /* defined (__TARGET__) */