#include "Box.h"
#include "TextureManager.h"
#include "Game.h"
#include "Util.h"
#include "PhysicsEnigine.h"

Box::Box()
{
}

Box::Box(glm::vec4 colour, float width, float height, Material material)
{
	this->colour = colour;
	
	SetWidth(width);
	SetHeight(height);
	SetMaterial(material);

	PhysicsEngine::Instance().AddObject(this);
}

Box::~Box()
= default;

void Box::Draw()
{
	Util::DrawCircle(GetTransform()->position, 2.0f);

	glm::vec2 position = glm::vec2(GetTransform()->position.x - (GetWidth() / 2), GetTransform()->position.y - (GetHeight() / 2));

	Util::DrawRect(position, GetWidth(), GetHeight(), colour);
}

void Box::Update()
{
}

void Box::Clean()
{
}
