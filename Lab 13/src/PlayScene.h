#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Button.h"
#include "Label.h"

#include "Target.h"
#include "Ground.h"
#include "Box.h"

#include "PhysicsEnigine.h"


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

	Box* m_pGroundBox;

	std::vector<Box*> m_pBoxes;
	std::vector<Target*> m_pCircles;

	Target* grabbedCircle;

	Ground* m_pGroundPlane;
	Ground* m_pGroundPlane2;

	bool RunPhysics = true;
};

#endif /* defined (__PLAY_SCENE__) */