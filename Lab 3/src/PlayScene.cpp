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

	//m_angleDegrees += 11;
	//if (m_angleDegrees > 360)
	//{
	//	m_angleDegrees -= 360;
	//}

	if (BallLaunched)
	{
		m_pBall->GetRigidBody()->velocity += m_pBall->GetRigidBody()->acceleration;
	}

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
		if (BallLaunched)
		{
			BallLaunched = 0;
			m_pBall->GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
			m_pBall->GetTransform()->position = m_startPosition;
			m_pBall->GetRigidBody()->acceleration.y = 0.0f;
		}
		else
		{
			BallLaunched = 1;
			//Code for launching ball
			m_pBall->GetRigidBody()->velocity = AngleMagnitudeToVec2(m_angleDegrees, m_speed);
			m_pBall->GetRigidBody()->acceleration.y = m_gravity;
		}
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
	m_pBall->GetTransform()->position = m_startPosition;


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
	ImGui::SliderFloat("Gravity", &m_gravity, -100, 100);
	ImGui::SliderFloat2("Start Position", &(m_startPosition.x), 0, 600);
	

	glm::vec2 velocity = AngleMagnitudeToVec2(m_angleDegrees, m_speed);
	
	ImGui::LabelText("Velocity", "x:%f, y:%f", velocity.x, velocity.y);


	ImGui::End();
}
