#include "PhysicsEnigine.h"
#include "RigidBody.h"
#include "GameObject.h"

void PhysicsEngine::AddObject(RigidBody* rigidBody)
{
	physicsObject.push_back(rigidBody);
}

void PhysicsEngine::Update()
{

	for each (RigidBody * rb in physicsObject)
	{
		//apply acceleration to velocity
		rb->velocity += gravityAcceleration * rb->gravityScale * fixedDeltatime;
		//apply friction
		rb->velocity *= rb->damping;
		//apply velocity
		rb->gameObject->GetTransform()->position += rb->velocity * fixedDeltatime;
	}
/*
	for (int i = 0; i < 3; i++)
	{
		for (int j = i + 1; j < 3; j++)
		{
			Check(i, j);
		}
	}

	

	for (auto it = physicsObject.begin(); it != physicsObject.end(); it++)
	{
		//deference to access the element
		RigidBody* rb(*it);

		auto it2 = it;
		it2++;
		for (; it2 != physicsObject.end(); it2++)
		{
			RigidBody* rb2 = (*it2);

			if (colliding(rb, rb2))
			{
				print;
			}
		}
	}
	*/
}
