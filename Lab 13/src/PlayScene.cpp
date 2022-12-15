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

	
	glm::vec2 circleCenter = m_pCircles.back()->GetTransform()->position;
	glm::vec2 rectanglePoint;
	
	glm::vec2 minRec = glm::vec2(m_pGroundBox->GetTransform()->position.x - m_pGroundBox->GetWidth() / 2, m_pGroundBox->GetTransform()->position.y - m_pGroundBox->GetHeight() / 2);
	glm::vec2 maxRec = glm::vec2(m_pGroundBox->GetTransform()->position.x + m_pGroundBox->GetWidth() / 2, m_pGroundBox->GetTransform()->position.y + m_pGroundBox->GetHeight() / 2);

	rectanglePoint = Util::ClampEdge(circleCenter, minRec, maxRec);
	glm::vec2 circlebounds = (Util::Normalize(rectanglePoint - circleCenter) * m_pCircles.back()->GetRadius()) + circleCenter;
	
	//Draw Circle To Rectangle collision line bounds
	Util::DrawLine(circleCenter, circlebounds);

	//Draw Rectangle Clamped point
	Util::DrawCircle(rectanglePoint, 3);

	//Draw Circle Bound Point
	Util::DrawCircle(circlebounds, 3, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

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

	///*------- Circles -------*/
	//circle 1
	m_pCircles.push_back(new Target(glm::vec4(0.7f, 0.0f, 0.0f, 1.0f), 30.0f, STONE));
	m_pCircles.back()->GetTransform()->position = glm::vec2(1200.0f, 100.0f);
	m_pCircles.back()->GetRigidBody()->name = "Red Circle";
	m_pCircles.back()->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(0.0f, -8.0f);
	//m_pCircles.back()->GetRigidBody()->affectedByPhysics = false;
	//m_pCircles.back()->GetRigidBody()->gravityScale = 0.0f;
	AddChild(m_pCircles.back());

	////circle 2
	//m_pCircles.push_back(new Target(glm::vec4(0.0f, 0.7f, 0.0f, 1.0f), 20.0f, STEEL));
	//m_pCircles.back()->GetTransform()->position = glm::vec2(300.0f, 100.0f);
	//m_pCircles.back()->GetRigidBody()->name = "Green Circle";
	//m_pCircles.back()->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(180.0f, 8.0f);
	//AddChild(m_pCircles.back());
	//
	////circle 3
	//m_pCircles.push_back(new Target(glm::vec4(0.0f, 0.0f, 0.7f, 1.0f), 45.0f, WOOD));
	//m_pCircles.back()->GetTransform()->position = glm::vec2(500.0f, 100.0f);
	//m_pCircles.back()->GetRigidBody()->name = "Blue Circle";
	//m_pCircles.back()->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(235.0f, 8.0f);

	//AddChild(m_pCircles.back());
	//
	////circle 4
	//m_pCircles.push_back(new Target(glm::vec4(0.75f, 0.7f, 0.0f, 1.0f), 10.0f, WOOD));
	//m_pCircles.back()->GetTransform()->position = glm::vec2(700.0f, 100.0f);
	//m_pCircles.back()->GetRigidBody()->name = "Yellow Circle";
	//m_pCircles.back()->GetRigidBody()->velocity = Util::AngleMagnitudeToVec2(150.0f, 8.0f);

	//AddChild(m_pCircles.back());
	
	
	/*------- AABB -------*/
	m_pGroundBox = new Box(glm::vec4(0.8f, 0.0f, 0.1f, 1.0f), 650.0f, 500.0f, STEEL);
	m_pGroundBox->GetTransform()->position = glm::vec2(640.0f, 650.0f);
	m_pGroundBox->GetRigidBody()->gravityScale = 0.0f;
	m_pGroundBox->GetRigidBody()->affectedByPhysics = false;
	AddChild(m_pGroundBox);

	//Box1
	m_pBoxes.push_back(new Box(glm::vec4(0.0f, 1.0f, 0.6f, 1.0f), 60.0f, 35.0f, STEEL));
	m_pBoxes.back()->GetTransform()->position = glm::vec2(620.0f, 550.0f);
	AddChild(m_pBoxes.back());

	//Box2
	m_pBoxes.push_back(new Box(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), 60.0f, 35.0f, RUBBER));
	m_pBoxes.back()->GetTransform()->position = glm::vec2(630.0f, 600.0f);
	AddChild(m_pBoxes.back());

	//Box3
	m_pBoxes.push_back(new Box(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), 60.0f, 35.0f, DIRT));
	m_pBoxes.back()->GetTransform()->position = glm::vec2(635.0f, 500.0f);
	AddChild(m_pBoxes.back());

	

	/*------- HalfPlane -------*/
	//m_pGroundPlane = new Ground();
	//AddChild(m_pGroundPlane);
	//m_pGroundPlane->GetTransform()->position = glm::vec2(400.0f, 700.0f);
	//m_pGroundPlane->SetMaterial(DIRT);

	//m_pGroundPlane2 = new Ground();
	//AddChild(m_pGroundPlane2);
	//m_pGroundPlane2->SetNormalAngle(0);
	//m_pGroundPlane2->GetTransform()->position.x = 10;
	//m_pGroundPlane->SetMaterial(STEEL);



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

	float posx = m_pBoxes.back()->GetTransform()->position.x;
	float posy = m_pBoxes.back()->GetTransform()->position.y;

	if (ImGui::SliderFloat("positionx", &posx, 0, 1024))
	{
		m_pBoxes.back()->GetTransform()->position.x = posx;
	}
	if (ImGui::SliderFloat("positiony", &posy, 0, 720))
	{
		m_pBoxes.back()->GetTransform()->position.y = posy;
	}

	float posx2 = m_pCircles.back()->GetTransform()->position.x;
	float posy2 = m_pCircles.back()->GetTransform()->position.y;

	if (ImGui::SliderFloat("positionx2", &posx2, 0, 1024))
	{
		m_pCircles.back()->GetTransform()->position.x = posx2;
	}
	if (ImGui::SliderFloat("positiony2", &posy2, 0, 720))
	{
		m_pCircles.back()->GetTransform()->position.y = posy2;
	}

	ImGui::Separator;
	ImGui::End();
}
