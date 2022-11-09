#include "Target.h"
#include "TextureManager.h"
#include "Game.h"
#include "Util.h"

Target::Target()
{
	TextureManager::Instance().Load("../Assets/sprites/Ball.png","ball");

	const auto size = TextureManager::Instance().GetTextureSize("ball");
	SetWidth(static_cast<int>(size.x));
	SetHeight(static_cast<int>(size.y));
	GetTransform()->position = glm::vec2(100.0f, 100.0f);
	GetRigidBody()->velocity = glm::vec2(0, 0);
	GetRigidBody()->isColliding = false;

	GetRigidBody()->radius = 30.0f;
	SetType(GameObjectType::CIRCLE);
}

Target::~Target()
= default;

void Target::Draw()
{
	// draw the target
	Util::DrawCircle(GetTransform()->position, GetRigidBody()->radius, glm::vec4(0.0f, 0.0f, 0.7f, 1.0f));
	//TextureManager::Instance().Draw("ball", GetTransform()->position, 0, 255, true);
}

void Target::Update()
{

}

void Target::Clean()
{
}