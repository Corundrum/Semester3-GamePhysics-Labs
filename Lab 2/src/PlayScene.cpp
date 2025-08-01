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

	glm::vec2 velocity = AngleMagnitudeToVec2(m_angleDegrees, m_speed);
	
	glm::vec2 endPosition = m_startPosition + velocity;

	Util::DrawLine(m_startPosition, endPosition, glm::vec4(1.0, 0.0, 0.0, 1.0));



	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	UpdateDisplayList();
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
	
	// Plane Sprite
	m_pPlaneSprite = new Plane();
	AddChild(m_pPlaneSprite);

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

	ImGui::SliderFloat("Angle", &m_angleDegrees, 0, 360);
	ImGui::SliderFloat("Speed", &m_speed, 0, 1000);
	ImGui::SliderFloat2("Start Position", &(m_startPosition.x), 0, 600);
	
	glm::vec2 velocity = AngleMagnitudeToVec2(m_angleDegrees, m_speed);
	
	ImGui::LabelText("Velocity", "x:%f, y:%f", velocity.x, velocity.y);


	ImGui::End();
}
