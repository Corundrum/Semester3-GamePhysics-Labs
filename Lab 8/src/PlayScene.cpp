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

	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_R))
	{
		m_pBall->GetTransform()->position = glm::vec2(100, 100);
		m_pBall->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(startAngle, startSpeed);

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

	/*------- HalfPlane -------*/
	m_pHalfplane = new HalfPlane();
	AddChild(m_pHalfplane);
	m_pHalfplane->GetRigidBody()->gravityScale = 0.0f;
	PhysicsEngine::Instance().AddPlaneObject(m_pHalfplane);
	m_pHalfplane->GetTransform()->position = glm::vec2(400, 500);
	
	/*------- Ball -------*/
	m_pBall = new Target;
	AddChild(m_pBall);
	PhysicsEngine::Instance().AddPhysicsObject(m_pBall->GetRigidBody());
	m_pBall->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(startAngle, startSpeed);
	



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
	
	ImGui::SliderFloat2("HalfPlane Position", &m_pHalfplane->GetTransform()->position.x, 0, 800);
	
	ImGui::SliderFloat("Angle", &startAngle, 0, 360);

	if (ImGui::SliderFloat("HalfPlane Orientation", &m_halfPlaneOrientation, 0, 360))
	{
		m_pHalfplane->SetNormalAngle(m_halfPlaneOrientation);
		//m_pHalfplane->m_normal = AngleMagnitudeToVec2(m_halfPlaneOrientation, 1.0f);
	}


	ImGui::Separator;

	

	ImGui::End();
}
