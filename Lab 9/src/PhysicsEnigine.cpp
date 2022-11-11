#include "PhysicsEnigine.h"
#include "CollisionShape.h"

#include "Util.h"
#include <iostream>


void PhysicsEngine::AddObject(CollisionShape* object)
{
	if (object->GetShape() == CIRCLE)
	{
		objects.push_front(object);
	}
	else if (object->GetShape() == HALFPLANE)
	{
		objects.push_back(object);
	}
}

bool PhysicsEngine::CircleCircleCheck(CollisionCircle* circle1, CollisionCircle* circle2)
{
	float distance = Util::Distance(circle2->GetTransform()->position, circle1->GetTransform()->position);
	float sum_radius = circle1->GetRadius() + circle2->GetRadius();

	if (distance <= sum_radius)
		return true;

	return false;
}

bool PhysicsEngine::CircleHalfPlaneCheck(CollisionCircle* circle, CollisionHalfPlane* half_plane)
{
	glm::vec2 relative_position = circle->GetTransform()->position - half_plane->GetTransform()->position;
	float distance = Util::Dot(relative_position, half_plane->GetNormalVector()) - circle->GetRadius();
	
	//Stopping the Ball
	if (distance <= 0)
	{
		//circle->GetTransform()->position -= half_plane->GetNormalVector() * distance;

		//Force of Normal
		glm::vec2 FNormal = -Util::Project(FGravity, half_plane->GetNormalVector());
		circle->GetRigidBody()->netForce += FNormal;
		
		//Friction V1
		float gravDotnormal = Util::Dot(FGravity, half_plane->GetNormalVector());
		glm::vec2 perpendicular = gravDotnormal * half_plane->GetNormalVector();
		glm::vec2 parallel = FGravity - perpendicular;

		float FNormalMagnitude = abs(gravDotnormal);

		glm::vec2 frictionDirection = -Util::Normalize(parallel);

		float k_friction = Util::Max(circle->GetRigidBody()->friction, half_plane->GetRigidBody()->friction);


		float FrictionMagnitude = Util::Min(k_friction * FNormalMagnitude, Util::Magnitude(parallel));

		

		glm::vec2 FFriction = FrictionMagnitude * frictionDirection;

		circle->GetRigidBody()->netForce += FFriction;




		//Friction V2
		//float k_friction = Util::Max(circle->GetRigidBody()->friction, half_plane->GetRigidBody()->friction); //0.2f
		//
		//glm::vec2 parallel = FGravity + FNormal; //Parallel Vector, Opposite of Friction

		//float FrictionMagnitude = Util::Min(k_friction * Util::Magnitude(FNormal), Util::Magnitude(parallel)); 
		//
		//glm::vec2 FFriction = FrictionMagnitude * -Util::Normalize(parallel);

		//circle->GetRigidBody()->netForce += FFriction;
	}

	//overlapping
	if (distance < 0)
	{
		circle->GetTransform()->position -= half_plane->GetNormalVector() * distance;
		return true;
	}
		
	
	return false;
}


void PhysicsEngine::Update()
{
	
	//Physics
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		CollisionShape* object1(*it);

		FGravity = object1->GetRigidBody()->mass * gravityAcceleration * object1->GetRigidBody()->gravityScale;
		object1->GetRigidBody()->netForce += FGravity;
		
		auto it2 = it;
		it2++;
		for (; it2 != objects.end(); it2++)
		{

			CollisionShape* object2(*it2);

			/*-------CIRCLE TO CIRCLE-------*/
			if (object1->GetShape() == CIRCLE && object2->GetShape() == CIRCLE)
			{
				if (CircleCircleCheck((CollisionCircle*)object1, (CollisionCircle*)object2))
				{
				
				}
			}

			/*-------CIRCLE TO HALFPLANE-------*/
			else if (object1->GetShape() == CIRCLE && object2->GetShape() == HALFPLANE)
			{
				if (CircleHalfPlaneCheck((CollisionCircle*)object1, (CollisionHalfPlane*)object2))
				{
					
				}
			}
		}

		object1->GetRigidBody()->acceleration = object1->GetRigidBody()->netForce / object1->GetRigidBody()->mass;
		object1->GetRigidBody()->velocity = object1->GetRigidBody()->acceleration * fixedDeltatime;
		object1->GetTransform()->position += object1->GetRigidBody()->velocity;

		if (object1->GetShape() == CIRCLE)
		{
			//std::cout << "Acceleration: " << object1->GetRigidBody()->acceleration.x << ", " << object1->GetRigidBody()->acceleration.y << std::endl;
			//std::cout << "Velocity: " << object1->GetRigidBody()->velocity.x << ", " << object1->GetRigidBody()->velocity.y << std::endl;
			//std::cout << "Net Force: " << object1->GetRigidBody()->netForce.x << ", " << object1->GetRigidBody()->netForce.y << std::endl;
		}

		//reset net force
		object1->GetRigidBody()->netForce = glm::vec2(0.0f, 0.0f);
	}
}
