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

	m_mousePosition = EventManager::Instance().GetMousePosition();
	
	if (grabbedCircle != nullptr)
	{
		grabbedCircle->GetTransform()->position = m_mousePosition;
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

	if (EventManager::Instance().MousePressed(1) && grabbedCircle == nullptr)
	{
		for (auto circle : m_pCircles)
		{
			float distance = Util::Distance(m_mousePosition, circle->GetTransform()->position);

			if (distance < circle->GetRadius())
			{
				grabbedCircle = circle;
				std::cout << "Grabbed the " << grabbedCircle->GetRigidBody()->name << std::endl;
				break;
			}
		}
	}


	if (EventManager::Instance().MouseReleased(1) && grabbedCircle != nullptr)
	{
		std::cout << "Released the " << grabbedCircle->GetRigidBody()->name << std::endl;

		grabbedCircle->GetRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
		grabbedCircle->GetRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		grabbedCircle = nullptr;
	}
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	/*------- Circles -------*/
	//circle 1
	m_pCircles.push_back(new Target(glm::vec4(0.7f, 0.0f, 0.0f, 1.0f), 30.0f, STONE));
	m_pCircles.back()->GetTransform()->position = glm::vec2(100.0f, 100.0f);
	m_pCircles.back()->GetRigidBody()->name = "Red Circle";
	m_pCircles.back()->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(0.0f, 8.0f);

	AddChild(m_pCircles.back());

	//circle 2
	m_pCircles.push_back(new Target(glm::vec4(0.0f, 0.7f, 0.0f, 1.0f), 20.0f, STEEL));
	m_pCircles.back()->GetTransform()->position = glm::vec2(300.0f, 100.0f);
	m_pCircles.back()->GetRigidBody()->name = "Green Circle";
	m_pCircles.back()->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(180.0f, 8.0f);
	AddChild(m_pCircles.back());
	
	//circle 3
	m_pCircles.push_back(new Target(glm::vec4(0.0f, 0.0f, 0.7f, 1.0f), 45.0f, WOOD));
	m_pCircles.back()->GetTransform()->position = glm::vec2(500.0f, 100.0f);
	m_pCircles.back()->GetRigidBody()->name = "Blue Circle";
	m_pCircles.back()->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(235.0f, 8.0f);

	AddChild(m_pCircles.back());
	
	//circle 4
	m_pCircles.push_back(new Target(glm::vec4(0.75f, 0.7f, 0.0f, 1.0f), 10.0f, WOOD));
	m_pCircles.back()->GetTransform()->position = glm::vec2(700.0f, 100.0f);
	m_pCircles.back()->GetRigidBody()->name = "Yellow Circle";
	m_pCircles.back()->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(150.0f, 8.0f);

	AddChild(m_pCircles.back());
	


	/*------- HalfPlane -------*/
	m_pGroundPlane = new Ground();
	AddChild(m_pGroundPlane);
	m_pGroundPlane->SetMaterial(DIRT);

	m_pGroundPlane2 = new Ground();
	AddChild(m_pGroundPlane2);
	m_pGroundPlane2->SetNormalAngle(0);
	m_pGroundPlane2->GetTransform()->position.x = 10;
	m_pGroundPlane->SetMaterial(STEEL);



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

	/*for (auto circle : m_pCircles)
	{
		ImGui::SliderFloat("Ball Friction", &circle->GetRigidBody()->friction, 0, 100);
	}*/

	ImGui::Separator;
	ImGui::End();
}
