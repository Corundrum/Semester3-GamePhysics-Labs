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
	//adding time up per frame
	t += dt;

	//plane 1
	m_pPlaneSprite->GetTransform()->position = glm::vec2
	(
		d + cos(t * a) * b,
		c + sin(t * a) * b
	);

	//plane 2
	m_pPlaneSprite2->GetTransform()->position = glm::vec2
	(
		m_pPlaneSprite2->GetTransform()->position.x + (-sin(t * a)) * a * b * dt,
		m_pPlaneSprite2->GetTransform()->position.y + (cos(t * a)) * a * b * dt
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

	m_pPlaneSprite2 = new Plane();
	AddChild(m_pPlaneSprite2);
	m_pPlaneSprite2->GetTransform()->position = glm::vec2
	(
		d + 100,
		c + 100
	);

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

	//Slider for Variable dt for delta time
	ImGui::SliderFloat("Delta Time", &dt, 0.0f, 1.0f);

	//Slider for Variable t for time
	ImGui::SliderFloat("Time", &t, 0.0f, 100.0f);
	
	//Slider for Variable a
	ImGui::SliderFloat("a", &a, 0.0f, 100.0f);

	//Slider for Variable b
	ImGui::SliderFloat("b", &b, 0.0f, 100.0f);

	//Slider for Variable c
	ImGui::SliderFloat("c", &c, 0.0f, 100.0f);

	//Slider for Variable d
	ImGui::SliderFloat("d", &d, 0.0f, 100.0f);


	ImGui::End();
}
