#pragma once
#ifndef __PHYSICS_OBJECT__
#define __PHYSICS_OBJECT__

#include "DisplayObject.h"

class PhysicsObject : public DisplayObject
{
public:
	PhysicsObject();
	virtual ~PhysicsObject();

	glm::vec2 AngleMagnitudeToVec2(float angle, float magnitude);

	float getGravityEffect();
	float getDampenEffect();
	float getSpeed();
	float getAngle();

	void setGravityEffect(float gravity);
	void setDampenEffect(float dampen);
	void setSpeed(float speed);
	void setAngle(float angle);

	//Inherited via Physics
	virtual void Draw() override = 0;
	virtual void Update() override;
	virtual void Clean() override = 0;


	float m_gravityEffect;
	float m_dampenEffect;
	float m_speed;
	float m_angle;
private:
	
};


#endif // !__PHYSICS_OBJECT__
