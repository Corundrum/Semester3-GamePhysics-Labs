#include "Target.h"
#include "TextureManager.h"
#include "Game.h"
#include "Util.h"

#include "PhysicsEnigine.h"

Target::Target()
{
	colour = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	PhysicsEngine::Instance().AddObject(this);
	SetRadius(30.0f);
	SetMaterial(DIRT);
}

Target::Target(glm::vec4 colour, float radius, Material material)
{
	this->colour = colour;
	SetRadius(radius);
	SetMaterial(material);
	PhysicsEngine::Instance().AddObject(this);
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