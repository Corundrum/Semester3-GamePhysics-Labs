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

	if (DrawHitbox)
	{
		Util::DrawCircle(m_pBall->GetTransform()->position, m_pBall->GetRigidBody()->radius, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		Util::DrawCircle(m_pPlane->GetTransform()->position, m_pPlane->GetRigidBody()->radius, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	}

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	if (RunPhysics)
	{
		PhysicsEngine::Instance().Update();
	}

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
		RunPhysics = !RunPhysics;
	}

	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_LSHIFT))
	{
		DrawHitbox = !DrawHitbox;
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
	PhysicsEngine::Instance().AddObject(m_pBall->GetRigidBody());
	m_pBall->GetRigidBody()->velocity = AngleMagnitudeToVec2(startAngle, startSpeed);


	m_pPlane = new Plane;
	AddChild(m_pPlane);
	PhysicsEngine::Instance().AddObject(m_pPlane->GetRigidBody());
	m_pPlane->GetTransform()->position = glm::vec2(600, 100);
	m_pPlane->GetRigidBody()->velocity = AngleMagnitudeToVec2(180, startSpeed);



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
	

	ImGui::Separator;

	

	ImGui::End();
}
