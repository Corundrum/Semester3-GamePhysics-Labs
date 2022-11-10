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

	//distance from line to ball -- Dot Product
	float distance = Util::Dot(relative_position, half_plane->GetNormalVector()) - circle->radius;

	if (distance <= 0)
		return true;

	return false;
}

void PhysicsEngine::Update()
{

	//Physics
	for (auto it = physicsObject.begin(); it != physicsObject.end(); it++)
	{
		RigidBody* rb(*it);
		glm::vec2 prevPos = rb->gameObject->GetTransform()->position;

		/*----------Force of Gravity----------*/
		glm::vec2 Fgravity = rb->mass * gravityVector;
		rb->netForce += Fgravity;
		

		for (auto it3 = physicsPlanes.begin(); it3 != physicsPlanes.end(); it3++)
		{
			HalfPlane* hp = (*it3);

			if (CircleHalfPlaneCheck(rb, hp))
			{
				rb->gameObject->GetTransform()->position -= hp->GetNormalVector() * (Util::Dot(rb->gameObject->GetTransform()->position - hp->GetTransform()->position, hp->GetNormalVector()) - rb->radius);

				/*----------Force of Normal----------*/
				glm::vec2 Fnormal = -(Util::Dot(Fgravity, hp->GetNormalVector()) * hp->GetNormalVector());
				/*----------Force of Friction----------*/
				glm::vec2 Ffriction = -(Fgravity + Fnormal);

				rb->netForce += Fnormal + Ffriction;
			}
		}
	
	

		rb->acceleration = (rb->netForce / rb->mass);
		rb->velocity += rb->acceleration;
		rb->gameObject->GetTransform()->position +=  rb->velocity * fixedDeltatime;

		

		std::cout << "Acceleration: (" << rb->acceleration.x << ", " << rb->acceleration.y << ")" << std::endl;
		std::cout << "Velocity: (" << rb->velocity.x << ", " << rb->velocity.y << ")" << std::endl;
		std::cout << "Net Force: (" << rb->netForce.x << ", " << rb->netForce.y << ")" << std::endl << std::endl;
		std::cout << "Delta: " << rb->gameObject->GetTransform()->position.y - prevPos.y << std::endl << std::endl;


		rb->netForce = glm::vec2(0.0f, 0.0f);
	}
}
