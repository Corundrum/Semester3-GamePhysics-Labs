#include "PhysicsEnigine.h"
#include "RigidBody.h"
#include "GameObject.h"
#include <iostream>

void PhysicsEngine::AddObject(RigidBody* rigidBody)
{
	physicsObject.push_back(rigidBody);
}

bool CheckCollision(RigidBody* pos1, RigidBody* pos2)
{
	if (
		pow(pos2->gameObject->GetTransform()->position.x - pos1->gameObject->GetTransform()->position.x, 2)
		+ pow(pos2->gameObject->GetTransform()->position.y - pos1->gameObject->GetTransform()->position.y, 2)
		<= pow(pos2->radius + pos1->radius, 2)
		)
	{
		return true;
	}
	else
	{
		return false;
	}
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

			if (CheckCollision(rb, rb2))
			{
				std::cout << "Colliding";
			}
		}
	}
	
}
