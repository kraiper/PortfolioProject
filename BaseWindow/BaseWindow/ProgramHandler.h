#pragma once
#include "stdafx.h"
#include "RenderEngine.h"
#include "LogicHandler.h"

class ProgramHandler
{
public:
	ProgramHandler(HWND* _windowHandle);
	~ProgramHandler();
	void PerFrame(float _deltaTime);

private:
	RenderEngine* renderEngine;
	LogicHandler* logicHandler;
};

