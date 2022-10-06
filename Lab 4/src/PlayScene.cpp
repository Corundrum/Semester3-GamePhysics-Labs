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
	
	ImGui::SliderFloat("Gravity Effect", &m_pBall->m_gravityEffect, 0, 10);
	ImGui::SliderFloat("Dampen Effect", &m_pBall->m_dampenEffect, 0, 10);
	ImGui::SliderFloat("Speed", &m_pBall->m_speed, 0, 100);
	ImGui::SliderFloat("Angle", &m_pBall->m_angle, 0, 360);

	ImGui::Text("Ball 2");

	ImGui::SliderFloat("Gravity Effect 2", &m_pBall2->m_gravityEffect, 0, 10);
	ImGui::SliderFloat("Dampen Effect 2", &m_pBall2->m_dampenEffect, 0, 10);
	ImGui::SliderFloat("Speed 2", &m_pBall2->m_speed, 0, 100);
	ImGui::SliderFloat("Angle 2", &m_pBall2->m_angle, 0, 360);
	
	ImGui::Text("Ball 3");

	ImGui::SliderFloat("Gravity Effect 3", &m_pBall3->m_gravityEffect, 0, 10);
	ImGui::SliderFloat("Dampen Effect 3", &m_pBall3->m_dampenEffect, 0, 10);
	ImGui::SliderFloat("Speed 3", &m_pBall3->m_speed, 0, 100);
	ImGui::SliderFloat("Angle 3", &m_pBall3->m_angle, 0, 360);
	

	ImGui::End();
}
