#pragma once
#ifndef __CIRCLE__
#define __CIRCLE__

#include "DisplayObject.h"

class Circle : public DisplayObject {
public:
	Circle();
	~Circle() override;

	// Inherited via GameObject
	virtual void Draw() override = 0;
	virtual void Update() override = 0;
	virtual void Clean() override = 0;

	void setRadius(float radius);
	float getRadius();

private:
	float m_radius = 30.0f;
};


#endif /* defined (__CIRCLE__) */