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

	auto GravityVector = PhysicsEngine::Instance().gravityAcceleration;
	auto PlaneNormalVector = m_pHalfplane->GetNormalVector();
	auto BallPosition = m_pBall->GetTransform()->position;

	//Force of Gravity
	glm::vec2 ForceGravity = m_pBall->GetRigidBody()->mass * GravityVector;

	//Force of Normal
	float dotProduct_GravityPlaneNormal = ((ForceGravity.x * PlaneNormalVector.x) + (ForceGravity.y * PlaneNormalVector.y));
	glm::vec2 Perpendicular = dotProduct_GravityPlaneNormal * PlaneNormalVector;
	glm::vec2 ForceNormal = -Perpendicular;
	
	//Force of Friction
	glm::vec2 Parallel = ForceGravity - Perpendicular;
	glm::vec2 ForceFriction = -Parallel;



	//Gravity Line
	Util::DrawLine(BallPosition, BallPosition + ForceGravity, glm::vec4(0.5f, 0.0f, 0.7f, 1.0f));
	m_pFGravity->GetTransform()->position = BallPosition + ForceGravity;

	//Normal Line
	Util::DrawLine(BallPosition, BallPosition + ForceNormal, glm::vec4(0.0f, 0.5f, 0.0f, 1.0f));
	m_pFNormal->GetTransform()->position = BallPosition + ForceNormal;

	//Friction Line
	Util::DrawLine(BallPosition, BallPosition + ForceFriction, glm::vec4(0.6f, 0.4f, 0.0f, 1.0f));
	m_pFFriction->GetTransform()->position = BallPosition + ForceFriction;

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
	

	m_pFGravity = new Label("Force of Gravity", "Consolas", 10, SDL_Color{ 64, 0, 100, 255 });
	m_pFGravity->SetParent(this);
	AddChild(m_pFGravity);

	m_pFNormal = new Label("Force of Normal", "Consolas", 10, SDL_Color{ 0, 80, 0, 255 });
	m_pFNormal->SetParent(this);
	AddChild(m_pFNormal);

	m_pFFriction = new Label("Force of Friction", "Consolas", 10, SDL_Color{ 80, 60, 0, 255 });
	m_pFFriction->SetParent(this);
	AddChild(m_pFFriction);

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

	if (ImGui::SliderFloat("HalfPlane Orientation", &m_halfPlaneOrientation, 0, 180))
	{
		m_pHalfplane->SetNormalAngle(m_halfPlaneOrientation);
		//m_pHalfplane->m_normal = AngleMagnitudeToVec2(m_halfPlaneOrientation, 1.0f);
	}

	float gravity = PhysicsEngine::Instance().gravityAcceleration.y;

	if (ImGui::SliderFloat("Gravity", &gravity, 0.0f, 50.0f))
	{
		PhysicsEngine::Instance().gravityAcceleration.y = gravity;
	}



	ImGui::Separator;

	

	ImGui::End();
}
