#include "Target.h"
#include "TextureManager.h"
#include "PhysicsManager.h"
#include "Game.h"


Target::Target()
{
	TextureManager::Instance().Load("../Assets/sprites/Ball.png","ball");

	const auto size = TextureManager::Instance().GetTextureSize("ball");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));
	GetTransform()->position = glm::vec2(100.0f, 100.0f);
	GetRigidBody()->velocity = glm::vec2(0, 0);
	GetRigidBody()->isColliding = false;

	SetType(GameObjectType::TARGET);
}

Target::~Target()
= default;

void Target::Draw()
{
	// draw the target
	TextureManager::Instance().Draw("ball", GetTransform()->position, 0, 255, true);
}

void Target::Update()
{

}

void Target::Clean()
{
}