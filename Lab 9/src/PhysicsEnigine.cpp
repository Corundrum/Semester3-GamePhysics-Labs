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
		//std::cout << angleVec.x << ", " << angleVec.y << std::endl;

		circle1->GetTransform()->position += angleVec * actual_distance;
		circle2->GetTransform()->position -= angleVec * actual_distance;



		std::cout << circle1->GetRigidBody()->name << " collided with " << circle2->GetRigidBody()->name << std::endl;
		
		float Cr = 1.0f;

		float m1 = circle1->GetRigidBody()->mass;
		float m2 = circle2->GetRigidBody()->mass;

		glm::vec2 u1 = circle1->GetRigidBody()->velocity;
		glm::vec2 u2 = circle2->GetRigidBody()->velocity;

		glm::vec2 mu1 = m1 * u1;
		glm::vec2 mu2 = m2 * u2;

		glm::vec2 initial_mass_velocity = mu1 + mu2;
		float total_mass = m1 + m2;



		glm::vec2 v1 = (initial_mass_velocity + (m2 * Cr * (u2 - u1))) / total_mass;
		glm::vec2 v2 = (initial_mass_velocity + (m1 * Cr * (u1 - u2))) / total_mass;

		circle1->GetRigidBody()->velocity = v1;
		circle2->GetRigidBody()->velocity = v2;



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
		//circle->GetTransform()->position -= half_plane->GetNormalVector() * distance;

		//Force of Normal
		glm::vec2 FNormal = -Util::Project(FGravity, half_plane->GetNormalVector());
		circle->GetRigidBody()->netForce += FNormal;
		
		//Friction V1
		/*float gravDotnormal = Util::Dot(FGravity, half_plane->GetNormalVector());
		glm::vec2 perpendicular = gravDotnormal * half_plane->GetNormalVector();
		glm::vec2 parallel = FGravity - perpendicular;

		float FNormalMagnitude = abs(gravDotnormal);

		glm::vec2 frictionDirection = -Util::Normalize(parallel);

		float k_friction = Util::Max(circle->GetRigidBody()->friction, half_plane->GetRigidBody()->friction);

		float FrictionMagnitude = Util::Min(k_friction * FNormalMagnitude, Util::Magnitude(parallel));

		glm::vec2 FFriction = FrictionMagnitude * frictionDirection;

		circle->GetRigidBody()->netForce += FFriction;*/

		//Friction V2
		float k_friction = Util::Max(circle->GetRigidBody()->friction, half_plane->GetRigidBody()->friction); //0.2f
		
		glm::vec2 parallel = FGravity + FNormal; //Parallel Vector, Opposite of Friction

		float FrictionMagnitude = Util::Min(k_friction * Util::Magnitude(FNormal), Util::Magnitude(parallel)); 
		
		glm::vec2 FFriction = FrictionMagnitude * -Util::Normalize(parallel);

		circle->GetRigidBody()->netForce += FFriction;

		//Conservation of Momentum
		float Cr = 0.8f;
		glm::vec2 u1 = circle->GetRigidBody()->velocity;

		glm::vec2 v1 = -Cr * (-u1);

		glm::vec2 iv1 = -Util::Normalize(v1);
		glm::vec2 div1 = half_plane->GetNormalVector() - iv1;
		glm::vec2 nv1 = half_plane->GetNormalVector() + div1;

		v1 = Util::Project(-v1, nv1);
		
		circle->GetRigidBody()->velocity = v1;

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
				CircleCircleCheck((CollisionCircle*)object1, (CollisionCircle*)object2);

			/*-------CIRCLE TO HALFPLANE-------*/
			else if (object1->GetShape() == CIRCLE && object2->GetShape() == HALFPLANE)
				CircleHalfPlaneCheck((CollisionCircle*)object1, (CollisionHalfPlane*)object2);
		}

		object1->GetRigidBody()->acceleration = object1->GetRigidBody()->netForce / object1->GetRigidBody()->mass;
		object1->GetRigidBody()->velocity += object1->GetRigidBody()->acceleration * fixedDeltatime;
		object1->GetRigidBody()->velocity *= damping;

		object1->GetTransform()->position += object1->GetRigidBody()->velocity;
		
		std::cout << object1->GetRigidBody()->velocity.x << ", " << object1->GetRigidBody()->velocity.y << std::endl;

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
