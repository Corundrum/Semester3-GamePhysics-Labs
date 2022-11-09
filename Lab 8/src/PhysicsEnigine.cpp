#include "PhysicsEnigine.h"
#include "RigidBody.h"
#include "GameObject.h"
#include "HalfPlane.h"
#include "Util.h"
#include <iostream>

void PhysicsEngine::AddPhysicsObject(RigidBody* rigidBody)
{
	physicsObject.push_back(rigidBody);
}

void PhysicsEngine::AddPlaneObject(HalfPlane* halfplane)
{
	physicsPlanes.push_back(halfplane);
}

float Vec2ToAngle(glm::vec2 vector)
{
	float angle = atan2(-vector.y, vector.x) * (180 / 3.14159);
	
	if (angle < 0)
	{
		angle += 360;
	}

	return angle;
}

bool CircleCircleCheck(RigidBody* pos1, RigidBody* pos2)
{
	float center_distance = pow(pos2->gameObject->GetTransform()->position.x - pos1->gameObject->GetTransform()->position.x, 2)
		+ pow(pos2->gameObject->GetTransform()->position.y - pos1->gameObject->GetTransform()->position.y, 2);

	float radius_combined = pow(pos2->radius + pos1->radius, 2);

	if (center_distance <= radius_combined)
	{
		float actual_distance = sqrt(center_distance) - sqrt(radius_combined);
		glm::vec2 angleVec = Util::Normalize(glm::vec2(pos2->gameObject->GetTransform()->position.x - pos1->gameObject->GetTransform()->position.x, pos2->gameObject->GetTransform()->position.y - pos1->gameObject->GetTransform()->position.y));
		//std::cout << angleVec.x << ", " << angleVec.y << std::endl;

		pos1->gameObject->GetTransform()->position += angleVec * actual_distance;
		pos2->gameObject->GetTransform()->position -= angleVec * actual_distance;

		glm::vec2 tempVel = pos1->velocity;
		pos1->velocity = pos2->velocity;
		pos2->velocity = tempVel;
		return true;
	}
	return false;
}

bool CircleHalfPlaneCheck(RigidBody* circle, HalfPlane* half_plane)
{
	//Position of circle relative to the line origin
	glm::vec2 relative_position = circle->gameObject->GetTransform()->position - half_plane->GetTransform()->position;

	//distance from line to ball
	float circle_distance_from_line = ((relative_position.x * half_plane->GetNormalVector().x) +
										(relative_position.y * half_plane->GetNormalVector().y)) - circle->radius;


	if (circle_distance_from_line < 0)
	{
		float oppositeDir = Vec2ToAngle(-circle->velocity);
		
		float angleDiff = half_plane->GetNormalAngle() - oppositeDir;

		float newAngle = half_plane->GetNormalAngle() + angleDiff;
	
		float speed = sqrt(pow(circle->velocity.x, 2) + pow(circle->velocity.y, 2));

		circle->gameObject->GetTransform()->position -= half_plane->GetNormalVector() * circle_distance_from_line;
		circle->velocity = Util::AngleMagnitudeToVec2(newAngle, speed);

		return true;
	}
	return false;
}

void PhysicsEngine::Update()
{

	//Physics
	for (auto it = physicsObject.begin(); it != physicsObject.end(); it++)
	{
		//deference to access the element
		RigidBody* rb(*it);

		//apply acceleration, damping and position
		//rb->velocity += gravityAcceleration * rb->gravityScale;
		//rb->velocity *= rb->damping;
		//rb->gameObject->GetTransform()->position += rb->velocity * fixedDeltatime;

		for (auto it3 = physicsPlanes.begin(); it3 != physicsPlanes.end(); it3++)
		{
			HalfPlane* hp = (*it3);

			rb->gameObject->GetTransform()->position = hp->GetTransform()->position;
			CircleHalfPlaneCheck(rb, hp);
		}
	}
}
