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
	{
		//prevent overlap
		float actual_distance = distance - sum_radius;
		glm::vec2 angleVec = Util::Normalize(glm::vec2(circle2->GetTransform()->position.x - circle1->GetTransform()->position.x, circle2->GetTransform()->position.y - circle1->GetTransform()->position.y));

		//circle1->GetTransform()->position += angleVec * actual_distance;
		//circle2->GetTransform()->position -= angleVec * actual_distance;

		
		//---------------Conservation of Momentum---------------//
		
		float m1 = circle1->GetRigidBody()->mass;
		float m2 = circle2->GetRigidBody()->mass;

		glm::vec2 u1 = circle1->GetRigidBody()->velocity;
		glm::vec2 u2 = circle2->GetRigidBody()->velocity;


		glm::vec2 contact_normal = Util::Normalize(circle2->GetTransform()->position - circle1->GetTransform()->position);

		float approaching_speed = Util::Dot(u2 - u1, contact_normal);

		if (approaching_speed < 0)
		{
			glm::vec2 approaching_velocity = approaching_speed * contact_normal;


			float restitution1 = ((circle1->GetRigidBody()->restitution * 2) + circle2->GetRigidBody()->restitution) / 3.0f;
			float restitution2 = ((circle2->GetRigidBody()->restitution * 2) + circle1->GetRigidBody()->restitution) / 3.0f;

			glm::vec2 impulse1 = 0.5f * (powf(restitution1, 2) * approaching_velocity * (m1 + m2)) * -contact_normal;
			glm::vec2 impulse2 = 0.5f * (powf(restitution2, 2) * approaching_velocity * (m1 + m2)) * contact_normal;

			circle1->GetRigidBody()->velocity = (u1 + impulse1) / m1;
			circle2->GetRigidBody()->velocity = (u2 + impulse2) / m2;
		}
		return true;
	}
	return false;
}

bool PhysicsEngine::CircleHalfPlaneCheck(CollisionCircle* circle, CollisionHalfPlane* half_plane)
{
	glm::vec2 relative_position = circle->GetTransform()->position - half_plane->GetTransform()->position;
	float distance = Util::Dot(relative_position, half_plane->GetNormalVector()) - circle->GetRadius();

	if (distance <= 0)
	{
		
		//Force of Normal
		glm::vec2 FNormal = -Util::Project(FGravity, half_plane->GetNormalVector());
		circle->GetRigidBody()->netForce += FNormal;

		//Friction V2
		float k_friction = Util::Max(circle->GetRigidBody()->friction, half_plane->GetRigidBody()->friction); //0.2f
		
		glm::vec2 parallel = FGravity + FNormal; //Parallel Vector, Opposite of Friction

		float FrictionMagnitude = Util::Min(k_friction * Util::Magnitude(FNormal), Util::Magnitude(parallel)); 
		
		glm::vec2 FFriction = FrictionMagnitude * -Util::Normalize(parallel);

		circle->GetRigidBody()->netForce += FFriction;

		//Conservation of Momentum
		
		float m1 = circle->GetRigidBody()->mass;

		glm::vec2 u1 = circle->GetRigidBody()->velocity;

		glm::vec2 contact_normal = half_plane->GetNormalVector();

		float approaching_speed = Util::Dot(u1, contact_normal);

		if (approaching_speed < 0)
		{
			glm::vec2 approaching_velocity = approaching_speed * contact_normal;

			
			float restitution = ((circle->GetRigidBody()->restitution * 2) + half_plane->GetRigidBody()->restitution) / 3.0f;

			glm::vec2 impulse = restitution * approaching_velocity * m1 * contact_normal;

			circle->GetRigidBody()->velocity = (u1 + impulse) / m1;
		}

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
				CircleCircleCheck((CollisionCircle*)object1, (CollisionCircle*)object2);

			/*-------CIRCLE TO HALFPLANE-------*/
			else if (object1->GetShape() == CIRCLE && object2->GetShape() == HALFPLANE)
				CircleHalfPlaneCheck((CollisionCircle*)object1, (CollisionHalfPlane*)object2);
		}

		object1->GetRigidBody()->acceleration = object1->GetRigidBody()->netForce / object1->GetRigidBody()->mass;
		object1->GetRigidBody()->velocity += object1->GetRigidBody()->acceleration * fixedDeltatime;
		object1->GetRigidBody()->velocity *= damping;

		object1->GetTransform()->position += object1->GetRigidBody()->velocity;
		
		
		if (object1->GetShape() == CIRCLE)
		{
			if (object1->GetTransform()->position.x < 0 + 30 || object1->GetTransform()->position.x > 1280 - 30)
			{
				object1->GetRigidBody()->velocity.x = -object1->GetRigidBody()->velocity.x;
			}
			else if (object1->GetTransform()->position.y < 0 + 30 || object1->GetTransform()->position.y > 720 - 30)
			{
				object1->GetRigidBody()->velocity.y = -object1->GetRigidBody()->velocity.y;
			}
		}
		

		//reset net force
		object1->GetRigidBody()->netForce = glm::vec2(0.0f, 0.0f);

	}
}
