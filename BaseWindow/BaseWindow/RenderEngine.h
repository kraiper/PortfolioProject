#pragma once
#include "stdafx.h"
#include "ButtonInput.h"
#include "Camera.h"
#include "PathfindingRender.h"
#include "LogicHandler.h"

extern Camera* Cam;
extern ButtonInput* buttonInput;

enum { GPUGen, Offline };


class RenderEngine
{
public:
	RenderEngine(HWND* _windowHandle, LogicHandler *_logicHandler);
	~RenderEngine();
	void Update();
	void Render(float _deltaTime);
private:
	
	PathfindingRender *pathfindingRender;

	int renderMethod = Offline;
	bool modeChange = false;

	LogicHandler *logicHandler;
};

