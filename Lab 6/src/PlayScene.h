#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "Target.h"
#include "PhysicsEnigine.h"
#include "HalfPlane.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene() override;

	// Scene LifeCycle Functions
	virtual void Draw() override;
	virtual void Update() override;
	virtual void Clean() override;
	virtual void HandleEvents() override;
	virtual void Start() override;
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Target* m_pBall;
	Target* m_pBall2;

	HalfPlane* m_pHalfplane;

	float m_halfPlaneOrientation = 270.0f;

	glm::vec2 ballTajectory;

	float startAngle = 315.0f;
	float startSpeed = 500.0f;

	// Input Control
	int m_pCurrentInputType{};
	void GetPlayerInput();
	void GetKeyboardInput();

	bool DrawHitbox = false;
	bool RunPhysics = false;

};

#endif /* defined (__PLAY_SCENE__) */