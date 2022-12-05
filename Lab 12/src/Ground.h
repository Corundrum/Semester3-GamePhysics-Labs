#pragma once
#include "CollisionShape.h"

class Ground : public CollisionHalfPlane
{
public:
	Ground();

	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;

private:

};

