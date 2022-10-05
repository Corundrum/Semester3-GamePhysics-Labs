#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "Label.h"
#include "Target.h"
#include <vector>

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
	Target* m_pBall3;

	glm::vec2 m_startPosition = glm::vec2(100,300);

	// Input Control
	int m_pCurrentInputType{};
	void GetPlayerInput();
	void GetKeyboardInput();
};

#endif /* defined (__PLAY_SCENE__) */