#include "Target.h"
#include "TextureManager.h"
#include "Game.h"
#include "Util.h"

#include "PhysicsEnigine.h"

Target::Target()
{
	colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	PhysicsEngine::Instance().AddObject(this);
	GetTransform()->position = glm::vec2(100.0f, 100.0f);
	GetRigidBody()->mass = 8.0f;//kg
}

Target::Target(glm::vec4 colour)
{
	this->colour = colour;
	PhysicsEngine::Instance().AddObject(this);
	GetTransform()->position = glm::vec2(100.0f, 100.0f);
	GetRigidBody()->mass = 8.0f;//kg
}

Target::~Target()
= default;

void Target::Draw()
{
	// draw the circle
	Util::DrawCircle(GetTransform()->position, GetRadius(), this->colour);
	
}

void Target::Update()
{

}

void Target::Clean()
{
}