#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "PhysicsManager.h"
#include "InputType.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

glm::vec2 AngleMagnitudeToVec2(float angle, float magnitude)
{
	return glm::vec2
	(	
		cos(-angle * Util::Deg2Rad) * magnitude, 
		sin(-angle * Util::Deg2Rad) * magnitude
	);
}

void PlayScene::Draw()
{
	DrawDisplayList();

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	PhysicsManager::Instance().RunPhysics();
	UpdateDisplayList();
	HandleEvents();

}

void PlayScene::Clean()
{
	RemoveAllChildren();
}


void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();

	GetPlayerInput();
	GetKeyboardInput();

}

void PlayScene::GetPlayerInput()
{

}

void PlayScene::GetKeyboardInput()
{
	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_SPACE))
	{
		m_pBall->GetTransform()->position = glm::vec2(100, 100);
		m_pBall->GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pBall->GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);

		m_pBall2->GetTransform()->position = glm::vec2(200, 100);
		m_pBall2->GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pBall2->GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);


		m_pBall3->GetTransform()->position = glm::vec2(300, 100);
		m_pBall3->GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pBall3->GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);


	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::START);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	m_pBall = new Target;
	AddChild(m_pBall);

	m_pBall2 = new Target;
	m_pBall2->GetTransform()->position.x += 100;
	AddChild(m_pBall2);

	m_pBall3 = new Target;
	m_pBall3->GetTransform()->position.x += 200;
	AddChild(m_pBall3);

	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("WindowWindow", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator;
	
	ImGui::Text("Gloabal");
	float gravity = PhysicsManager::Instance().getGravity();
	float drag = PhysicsManager::Instance().getDrag();
	ImGui::SliderFloat("Gravity", &gravity, 0, 100);
	ImGui::SliderFloat("Drag", &drag, -1.0f, 0.0f);
	PhysicsManager::Instance().setGravity(gravity);
	PhysicsManager::Instance().setDrag(drag);

	ImGui::Separator;

	ImGui::Text("Ball 1");
	float grav_eff = m_pBall->getGravityEffect();
	float damp_eff = m_pBall->getDampenEffect();
	float speed = m_pBall->getSpeed();
	float angle = m_pBall->getAngle();
	ImGui::SliderFloat("Gravity Effect", &grav_eff, 0, 10);
	ImGui::SliderFloat("Dampen Effect", &damp_eff, 0, 10);
	ImGui::SliderFloat("Speed", &speed, 0, 100);
	ImGui::SliderFloat("Angle", &angle, 0, 360);
	m_pBall->setGravityEffect(grav_eff);
	m_pBall->setDampenEffect(damp_eff);
	m_pBall->setSpeed(speed);
	m_pBall->setAngle(angle);
	


	ImGui::End();
}
