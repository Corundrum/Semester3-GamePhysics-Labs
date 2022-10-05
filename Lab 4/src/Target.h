#pragma once
#ifndef __TARGET__
#define __TARGET__

#include "PhysicsObject.h"

class Target final : public PhysicsObject {
public:
	Target();
	~Target() override;

	// Inherited via GameObject
	virtual void Draw() override;
	virtual void Update();
	virtual void Clean() override;

private:
};


#endif /* defined (__TARGET__) */