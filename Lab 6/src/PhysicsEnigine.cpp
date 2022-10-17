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

bool CircleCircleCheck(RigidBody* pos1, RigidBody* pos2)
{
	if (
		pow(pos2->gameObject->GetTransform()->position.x - pos1->gameObject->GetTransform()->position.x, 2)
		+ pow(pos2->gameObject->GetTransform()->position.y - pos1->gameObject->GetTransform()->position.y, 2)
		<= pow(pos2->radius + pos1->radius, 2)
		)
	{
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

	//trajectory of circle after colliding with half plane
	glm::vec2 ballTrajectory = Util::Normalize(circle->gameObject->GetRigidBody()->velocity) + half_plane->GetNormalVector();

	if (circle_distance_from_line < 0)
	{
		circle->gameObject->GetTransform()->position -= half_plane->GetNormalVector() * circle_distance_from_line;
		circle->velocity *= ballTrajectory;

		return true;
	}
	return false;
}

void PhysicsEngine::Update()
{
	//it = iterator
	for (auto it = physicsObject.begin(); it != physicsObject.end(); it++)
	{
		//deference to access the element
		RigidBody* rb(*it);

		//apply acceleration to velocity
		rb->velocity += gravityAcceleration * rb->gravityScale * fixedDeltatime;
		//apply friction
		rb->velocity *= rb->damping;
		//apply velocity
		rb->gameObject->GetTransform()->position += rb->velocity * fixedDeltatime;

		//collision loop
		//set it2 to the iterator
		auto it2 = it;
		//set it2 to the next in line along the list
		it2++;
		for (; it2 != physicsObject.end(); it2++)
		{
			RigidBody* rb2 = (*it2);
			if (CircleCircleCheck(rb, rb2))
			{
				std::cout << "Circle Circle Collision." << std::endl;
			}
		}
		for (auto it3 = physicsPlanes.begin(); it3 != physicsPlanes.end(); it3++)
		{
			HalfPlane* hp = (*it3);
			if (CircleHalfPlaneCheck(rb, hp))
			{
				std::cout << "Circle Halfplane Collision." << std::endl;
			}
		}
	}

	//std::cout << "Size: " << physicsObject.size() << std::endl;
	
}
