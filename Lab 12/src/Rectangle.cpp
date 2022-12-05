#include "Rectangle.h"
#include "TextureManager.h"
#include "Game.h"
#include "Util.h"
#include "PhysicsEnigine.h"

Rectangle::Rectangle()
{
}

Rectangle::Rectangle(glm::vec4 colour, float width, float height, Material material)
{
	this->colour = colour;
	SetWidth(width);
	SetHeight(height);
	//PhysicsEngine::Instance().AddObject(this);
}

Rectangle::~Rectangle() 
= default;

void Rectangle::Draw()
{
	Util::DrawCircle(GetTransform()->position, 2.0f);
	Util::DrawRect(GetTransform()->position, GetWidth(), GetHeight(), colour);
}

void Rectangle::Update()
{
}

void Rectangle::Clean()
{
}
