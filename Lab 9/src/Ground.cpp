#include "Ground.h"
#include "Util.h"

#include "PhysicsEnigine.h"

Ground::Ground()
{
	PhysicsEngine::Instance().AddObject(this);

	GetRigidBody()->gravityScale = 0.0f;
	GetTransform()->position = glm::vec2(400.0f, 400.0f);
	SetNormalAngle(GetNormalAngle());
	GetRigidBody()->friction = 1.0f;
	GetRigidBody()->mass = 1000000.0f;
}

void Ground::Draw()
{
	//Draw Normal
	glm::vec2 start = GetTransform()->position;
	glm::vec2 end = start + (GetNormalVector() * 50.0f);
	Util::DrawLine(start, end, glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));

	//Draw Plane
	Util::DrawLine(start - (GetPlaneVector() * 10000.0f), start + (GetPlaneVector() * 10000.0f), glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));
}

void Ground::Update()
{

}

void Ground::Clean()
{
}
