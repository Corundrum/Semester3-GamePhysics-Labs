#include "PhysicsEnigine.h"
#include "CollisionShape.h"
#include "CollisionManager.h"

#include "Util.h"
#include <iostream>


void PhysicsEngine::AddObject(CollisionShape* object)
{
	objects.push_front(object);
}

bool PhysicsEngine::CircleCircleCheck(CollisionCircle* circle1, CollisionCircle* circle2)
{
	float distance = Util::Distance(circle2->GetTransform()->position, circle1->GetTransform()->position);
	float sum_radius = circle1->GetRadius() + circle2->GetRadius();

	float overlap = sum_radius - distance;

	if (overlap >= 0)
	{

		float m1 = circle1->GetRigidBody()->mass;
		float m2 = circle2->GetRigidBody()->mass;

		glm::vec2 u1 = circle1->GetRigidBody()->velocity;
		glm::vec2 u2 = circle2->GetRigidBody()->velocity;

		//---------------Overlap---------------//

		glm::vec2 contact_normal = Util::Normalize(circle2->GetTransform()->position - circle1->GetTransform()->position);
		glm::vec2 minimum_translation = contact_normal * overlap;

		float percent1 = m2 / (m1 + m2);
		float percent2 = m1 / (m1 + m2);

		circle1->GetTransform()->position -= minimum_translation * percent1;
		circle2->GetTransform()->position += minimum_translation * percent2;



		//---------------Conservation of Momentum---------------//
		
		float approaching_speed = Util::Dot(u2 - u1, contact_normal);

		if (approaching_speed < 0)
		{
			float restitution1 = ((circle1->GetRigidBody()->restitution * 2.0f) + circle2->GetRigidBody()->restitution) / 3.0f;
			float restitution2 = (circle1->GetRigidBody()->restitution + (circle2->GetRigidBody()->restitution * 2.0f)) / 3.0f;


			float impulse1 = -(1.0f + restitution1) * approaching_speed * m1 * m2 / (m1 + m2);
			float impulse2 = -(1.0f + restitution2) * approaching_speed * m1 * m2 / (m1 + m2);

			circle1->GetRigidBody()->velocity += (impulse1 / m1) * -contact_normal;
			circle2->GetRigidBody()->velocity += (impulse2 / m2) * contact_normal;
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
		//---------------Force of Normal---------------//
		glm::vec2 FNormal = -Util::Project(FGravity, half_plane->GetNormalVector());
		circle->GetRigidBody()->netForce += FNormal;


		//---------------Conservation of Momentum---------------//		
		float m1 = circle->GetRigidBody()->mass;
		
		glm::vec2 u1 = circle->GetRigidBody()->velocity;

		float approaching_speed = Util::Dot(-u1, half_plane->GetNormalVector());

		if (approaching_speed > 0)
		{
			float restitution = ((circle->GetRigidBody()->restitution * 2.0f) + half_plane->GetRigidBody()->restitution) / 3.0f;

			float impulse = -(1.0f + restitution) * approaching_speed;

			circle->GetRigidBody()->velocity += (impulse) * -half_plane->GetNormalVector();
		}



		//---------------Force of Friction---------------//

		float k_friction = Util::Max(circle->GetRigidBody()->friction, half_plane->GetRigidBody()->friction); //0.2f

		glm::vec2 parallel = FGravity + FNormal; //Parallel Vector, Opposite of Friction

		float FrictionMagnitude = Util::Min(k_friction * Util::Magnitude(FNormal), Util::Magnitude(parallel));

		glm::vec2 FFriction = FrictionMagnitude * -Util::Normalize(parallel);

		circle->GetRigidBody()->netForce += FFriction;



		//---------------Overlap---------------//

		relative_position = circle->GetTransform()->position - half_plane->GetTransform()->position;
		distance = Util::Dot(relative_position, half_plane->GetNormalVector()) - circle->GetRadius();
		if (distance < 0)
		{
			circle->GetTransform()->position -= distance * half_plane->GetNormalVector();
		}
		return true;
	}
	return false;
}

bool PhysicsEngine::CircleAABBCheck(CollisionCircle* circle, CollisionAABB* aabb)
{
	//--------------- COLLISION CHECK ---------------//

	glm::vec2 circleCenter = circle->GetTransform()->position;

	glm::vec2 rectanglePoint;
	
	glm::vec2 minRec = glm::vec2(aabb->GetTransform()->position.x - aabb->GetWidth() / 2, aabb->GetTransform()->position.y - aabb->GetHeight() / 2);
	glm::vec2 maxRec = glm::vec2(aabb->GetTransform()->position.x + aabb->GetWidth() / 2, aabb->GetTransform()->position.y + aabb->GetHeight() / 2);

	rectanglePoint = Util::ClampEdge(circleCenter, minRec, maxRec);

	//rectanglePoint.x = Util::Clamp(circleCenter.x, aabb->GetTransform()->position.x - aabb->GetWidth() / 2, aabb->GetTransform()->position.x + aabb->GetWidth() / 2);
	//rectanglePoint.y = Util::Clamp(circleCenter.y, aabb->GetTransform()->position.y - aabb->GetHeight() / 2, aabb->GetTransform()->position.y + aabb->GetHeight() / 2);

	glm::vec2 circlebounds = (Util::Normalize(rectanglePoint - circleCenter) * circle->GetRadius()) + circleCenter;
	
	glm::vec2 aabbpos = aabb->GetTransform()->position;
	

	bool exitflag = false;
	//Collision Check early exit
	if (circleCenter.x < aabbpos.x - aabb->GetWidth() / 2 ||
		circleCenter.x > aabbpos.x + aabb->GetWidth() / 2 ||
		circleCenter.y < aabbpos.y - aabb->GetHeight() / 2 ||
		circleCenter.y > aabbpos.y + aabb->GetHeight() / 2)
	{
		exitflag = true;
	}
	if (!CollisionManager::LineRectCheck(circleCenter, circlebounds, minRec, aabb->GetWidth(), aabb->GetHeight()) && exitflag)
	{
		return false;
	}

	//--------------- OVERLAP ---------------//
	float xoverlap = rectanglePoint.x - circlebounds.x;
	float yoverlap = rectanglePoint.y - circlebounds.y;

	glm::vec2 minimum_translation;

	if (abs(xoverlap) > abs(yoverlap))
	{
		minimum_translation = glm::vec2(xoverlap, 0.0f);
	}
	else
	{
		minimum_translation = glm::vec2(0.0f, yoverlap);
	}

	float m1 = circle->GetRigidBody()->mass;
	float m2 = aabb->GetRigidBody()->mass;

	glm::vec2 u1 = circle->GetRigidBody()->velocity;
	glm::vec2 u2 = aabb->GetRigidBody()->velocity;

	if (circle->GetRigidBody()->affectedByPhysics && !aabb->GetRigidBody()->affectedByPhysics)
	{
		circle->GetTransform()->position += minimum_translation;
	}
	else if (!circle->GetRigidBody()->affectedByPhysics && aabb->GetRigidBody()->affectedByPhysics)
	{
		aabb->GetTransform()->position -= minimum_translation;
	}
	else if (circle->GetRigidBody()->affectedByPhysics && aabb->GetRigidBody()->affectedByPhysics)
	{
		float percent1 = m2 / (m1 + m2);
		float percent2 = m1 / (m1 + m2);

		circle->GetTransform()->position += minimum_translation * percent2;
		aabb->GetTransform()->position -= minimum_translation * percent1;
	}

	//---------------Conservation of Momentum---------------//
	glm::vec2 contact_normal = -Util::Normalize(minimum_translation);
	
	float approaching_speed = Util::Dot(u2 - u1, contact_normal);

	if (approaching_speed < 0)
	{
		if (circle->GetRigidBody()->affectedByPhysics && aabb->GetRigidBody()->affectedByPhysics)
		{
			float restitution1 = ((circle->GetRigidBody()->restitution * 2.0f) + aabb->GetRigidBody()->restitution) / 3.0f;
			float restitution2 = (circle->GetRigidBody()->restitution + (aabb->GetRigidBody()->restitution * 2.0f)) / 3.0f;


			float impulse1 = -(1.0f + restitution1) * approaching_speed * m1 * m2 / (m1 + m2);
			float impulse2 = -(1.0f + restitution2) * approaching_speed * m1 * m2 / (m1 + m2);

			circle->GetRigidBody()->velocity += (impulse1 / m1) * -contact_normal;
			aabb->GetRigidBody()->velocity += (impulse2 / m2) * contact_normal;
		}
		else if (!circle->GetRigidBody()->affectedByPhysics && aabb->GetRigidBody()->affectedByPhysics)
		{

			float restitution = ((aabb->GetRigidBody()->restitution * 2.0f) + circle->GetRigidBody()->restitution) / 3.0f;

			float impulse = -(1.0f + restitution) * approaching_speed;

			aabb->GetRigidBody()->velocity += (impulse)*contact_normal;

		}
		else if (circle->GetRigidBody()->affectedByPhysics && !aabb->GetRigidBody()->affectedByPhysics)
		{
			float restitution = ((circle->GetRigidBody()->restitution * 2.0f) + aabb->GetRigidBody()->restitution) / 3.0f;

			float impulse = -(1.0f + restitution) * approaching_speed;

			circle->GetRigidBody()->velocity -= (impulse) * contact_normal;
		}

		}

	return true;
}

bool PhysicsEngine::AABBAABBCheck(CollisionAABB* aabb1, CollisionAABB* aabb2)
{	
	float xdistance = aabb2->GetTransform()->position.x - aabb1->GetTransform()->position.x;
	float ydistance = aabb2->GetTransform()->position.y - aabb1->GetTransform()->position.y;
	
	float xsum = (aabb1->GetWidth() / 2) + (aabb2->GetWidth() / 2);
	float ysum = (aabb1->GetHeight() / 2) + (aabb2->GetHeight() / 2);

	float xoverlap = xsum - abs(xdistance);
	float yoverlap = ysum - abs(ydistance);

	if (xoverlap < 0 || yoverlap < 0)
	{
		return false;
	}

	float m1 = aabb1->GetRigidBody()->mass;
	float m2 = aabb2->GetRigidBody()->mass;

	glm::vec2 u1 = aabb1->GetRigidBody()->velocity;
	glm::vec2 u2 = aabb2->GetRigidBody()->velocity;

	//---------------Overlap---------------//

	glm::vec2 contact_normal;
	glm::vec2 minimum_translation;

	if (xoverlap < yoverlap)
	{
		contact_normal = glm::vec2(Util::Sign(xdistance), 0.0f);
		minimum_translation = contact_normal * xoverlap;
	}
	else
	{
		contact_normal = glm::vec2(0.0f, Util::Sign(ydistance));
		minimum_translation = contact_normal * yoverlap;
	}
	
	
	if (aabb1->GetRigidBody()->affectedByPhysics && aabb2->GetRigidBody()->affectedByPhysics)
	{
		float percent1 = m2 / (m1 + m2);
		float percent2 = m1 / (m1 + m2);

		aabb1->GetTransform()->position -= minimum_translation * percent1;
		aabb2->GetTransform()->position += minimum_translation * percent2;
	}
	else if (!aabb1->GetRigidBody()->affectedByPhysics && aabb2->GetRigidBody()->affectedByPhysics)
	{
		aabb2->GetTransform()->position += minimum_translation;
	}
	else if (aabb1->GetRigidBody()->affectedByPhysics && !aabb2->GetRigidBody()->affectedByPhysics)
	{
		aabb1->GetTransform()->position -= minimum_translation;
	}

	//---------------Conservation of Momentum---------------//

	float approaching_speed = Util::Dot(u2 - u1, contact_normal);

	if (approaching_speed < 0)
	{
		if (aabb1->GetRigidBody()->affectedByPhysics && aabb2->GetRigidBody()->affectedByPhysics)
		{
			float restitution1 = ((aabb1->GetRigidBody()->restitution * 2.0f) + aabb2->GetRigidBody()->restitution) / 3.0f;
			float restitution2 = (aabb1->GetRigidBody()->restitution + (aabb2->GetRigidBody()->restitution * 2.0f)) / 3.0f;


			float impulse1 = -(1.0f + restitution1) * approaching_speed * m1 * m2 / (m1 + m2);
			float impulse2 = -(1.0f + restitution2) * approaching_speed * m1 * m2 / (m1 + m2);

			aabb1->GetRigidBody()->velocity += (impulse1 / m1) * -contact_normal;
			aabb2->GetRigidBody()->velocity += (impulse2 / m2) * contact_normal;
		}
		else if (!aabb1->GetRigidBody()->affectedByPhysics && aabb2->GetRigidBody()->affectedByPhysics)
		{
			
			float restitution = ((aabb2->GetRigidBody()->restitution * 2.0f) + aabb1->GetRigidBody()->restitution) / 3.0f;

			float impulse = -(1.0f + restitution) * approaching_speed;

			aabb2->GetRigidBody()->velocity += (impulse) * contact_normal;
			
		}
		else if (aabb1->GetRigidBody()->affectedByPhysics && !aabb2->GetRigidBody()->affectedByPhysics)
		{
			float restitution = ((aabb1->GetRigidBody()->restitution * 2.0f) + aabb2->GetRigidBody()->restitution) / 3.0f;

			float impulse = -(1.0f + restitution) * approaching_speed;

			aabb1->GetRigidBody()->velocity -= (impulse)*contact_normal;
		}
	
	}

	return true;
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
			else if (object1->GetShape() == HALFPLANE && object2->GetShape() == CIRCLE)
				CircleHalfPlaneCheck((CollisionCircle*)object2, (CollisionHalfPlane*)object1);

			/*-------AABB TO AABB-------*/
			else if (object1->GetShape() == AABB && object2->GetShape() == AABB)
				AABBAABBCheck((CollisionAABB*)object1, (CollisionAABB*)object2);

			/*-------CIRCLE TO AABB-------*/
			else if (object1->GetShape() == CIRCLE && object2->GetShape() == AABB)
				CircleAABBCheck((CollisionCircle*)object1, (CollisionAABB*)object2);
			else if (object1->GetShape() == AABB && object2->GetShape() == CIRCLE)
				CircleAABBCheck((CollisionCircle*)object2, (CollisionAABB*)object1);

		}
		
		object1->GetRigidBody()->acceleration = object1->GetRigidBody()->netForce / object1->GetRigidBody()->mass;
		object1->GetRigidBody()->velocity += object1->GetRigidBody()->acceleration * fixedDeltatime;
		object1->GetRigidBody()->velocity *= damping;

		object1->GetTransform()->position += object1->GetRigidBody()->velocity;
		
		
	

		//reset net force
		object1->GetRigidBody()->netForce = glm::vec2(0.0f, 0.0f);

	}
}
