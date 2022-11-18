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

	for (auto circle : m_pCircles)
	{
		glm::vec2 FGravity = circle->GetRigidBody()->mass * PhysicsEngine::Instance().gravityAcceleration;
		glm::vec2 FNormal = -Util::Project(FGravity, m_pGroundPlane->GetNormalVector());
		
		//friction
		float NormalMag = Util::Dot(FGravity, m_pGroundPlane->GetNormalVector());
		glm::vec2 perp = NormalMag * m_pGroundPlane->GetNormalVector();
		glm::vec2 parallel = FGravity - perp;
		float FNormalMagnitude = abs(NormalMag);
		glm::vec2 frictionDirection = -Util::Normalize(parallel);
		float k_friction = Util::Max(circle->GetRigidBody()->friction, m_pGroundPlane->GetRigidBody()->friction);
		float FrictionMagnitude = Util::Min(k_friction * FNormalMagnitude, Util::Magnitude(parallel));
		glm::vec2 FFriction = FrictionMagnitude * frictionDirection;

		Util::DrawLine(circle->GetTransform()->position, circle->GetTransform()->position + FFriction, glm::vec4(1.0f, 0.7f, 0.0f, 1.0f));
		Util::DrawLine(circle->GetTransform()->position, circle->GetTransform()->position + parallel, glm::vec4(0.0f, 0.7f, 0.7f, 1.0f));
		Util::DrawLine(circle->GetTransform()->position, circle->GetTransform()->position + circle->GetRigidBody()->velocity, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		Util::DrawLine(circle->GetTransform()->position, circle->GetTransform()->position + FGravity, glm::vec4(0.6f, 0.0f, 0.7f, 1.0f));
		Util::DrawLine(circle->GetTransform()->position, circle->GetTransform()->position + FNormal, glm::vec4(0.0f, 0.7f, 0.0f, 1.0f));
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


}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	/*------- Circles -------*/
	m_pCircles.push_back(new Target(glm::vec4(0.7f, 0.0f, 0.0f, 1.0f)));
	m_pCircles.back()->GetTransform()->position = glm::vec2(100.0f, 100.0f);
	m_pCircles.back()->GetRigidBody()->name = "Red Circle";
	m_pCircles.back()->GetRigidBody()->friction = 0.1f;
	m_pCircles.back()->GetRigidBody()->mass = 2.0f;
	m_pCircles.back()->GetRigidBody()->restitution = 0.8f;

	m_pCircles.back()->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(5.0f, 12.0f);

	AddChild(m_pCircles.back());


	m_pCircles.push_back(new Target(glm::vec4(0.0f, 0.7f, 0.0f, 1.0f)));
	m_pCircles.back()->GetTransform()->position = glm::vec2(300.0f, 100.0f);
	m_pCircles.back()->GetRigidBody()->name = "Green Circle";
	m_pCircles.back()->GetRigidBody()->friction = 0.8f;
	m_pCircles.back()->GetRigidBody()->mass = 20.0f;
	m_pCircles.back()->GetRigidBody()->restitution = 0.0f;

	m_pCircles.back()->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(180.0f, 8.0f);


	AddChild(m_pCircles.back());
	

	m_pCircles.push_back(new Target(glm::vec4(0.0f, 0.0f, 0.7f, 1.0f)));
	m_pCircles.back()->GetTransform()->position = glm::vec2(500.0f, 100.0f);
	m_pCircles.back()->GetRigidBody()->name = "Blue Circle";
	m_pCircles.back()->GetRigidBody()->friction = 0.1f;
	m_pCircles.back()->GetRigidBody()->mass = 4.0f;
	m_pCircles.back()->GetRigidBody()->restitution = 0.5f;
	
	m_pCircles.back()->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(0.0f, 8.0f);

	AddChild(m_pCircles.back());
	

	m_pCircles.push_back(new Target(glm::vec4(0.75f, 0.7f, 0.0f, 1.0f)));
	m_pCircles.back()->GetTransform()->position = glm::vec2(700.0f, 100.0f);
	m_pCircles.back()->GetRigidBody()->name = "Yellow Circle";
	m_pCircles.back()->GetRigidBody()->friction = 0.8f;
	m_pCircles.back()->GetRigidBody()->mass = 8.0f;
	m_pCircles.back()->GetRigidBody()->restitution = 0.6f;

	m_pCircles.back()->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(180.0f, 8.0f);

	AddChild(m_pCircles.back());
	

	//std::cout << m_pCircles.back()->GetRigidBody()->velocity.x << ", " << m_pCircles.back()->GetRigidBody()->velocity.y << std::endl;

	/*------- HalfPlane -------*/
	m_pGroundPlane = new Ground();
	AddChild(m_pGroundPlane);
	m_pGroundPlane->GetRigidBody()->restitution = 0.8f;

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



	ImGui::SliderFloat2("Ground Position", &m_pGroundPlane->GetTransform()->position.x, 0, 800);

	float GroundAngle = m_pGroundPlane->GetNormalAngle();
	if (ImGui::SliderFloat("Ground Angle", &GroundAngle, 0, 360))
	{
		m_pGroundPlane->SetNormalAngle(GroundAngle);
	}

	ImGui::SliderFloat("Ground Friction", &m_pGroundPlane->GetRigidBody()->friction, 0, 100);

	ImGui::SliderFloat("Ball Friction", &m_pCircles.back()->GetRigidBody()->friction, 0, 100);

	ImGui::Separator;
	ImGui::End();
}
