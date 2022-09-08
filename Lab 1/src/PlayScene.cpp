#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
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
	t += dt;

	m_pPlaneSprite->GetTransform()->position = glm::vec2
	(
		d + cos(t * a) * b,
		c + sin(t * a) * b
	);

	UpdateDisplayList();
}

void PlayScene::Clean()
{
	RemoveAllChildren();
}


void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";
	
	// Plane Sprite
	m_pPlaneSprite = new Plane();
	AddChild(m_pPlaneSprite);

	// Player Sprite
	m_pPlayer = new Player();
	AddChild(m_pPlayer);

	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });
}

void PlayScene::GUI_Function() 
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Renamed IMGUI window", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Separator();

	ImGui::SliderFloat("time", &t, 0.0f, 100.0f);
	
	
	ImGui::End();
}
