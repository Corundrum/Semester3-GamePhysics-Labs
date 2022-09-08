#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"

const float PHYSICS_TIMESTAMP = 1.0f / 60.0f;

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

	Plane* m_pPlaneSprite{};
	Player* m_pPlayer{};

	

	float t = 0.0f;
	float dt = PHYSICS_TIMESTAMP;
	float a = 1.0f;
	float b = 100.0f;
	float c = 100.0f;
	float d = 100.0f;

};

#endif /* defined (__PLAY_SCENE__) */