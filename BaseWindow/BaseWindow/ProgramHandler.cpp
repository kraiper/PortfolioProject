#include "ProgramHandler.h"



ProgramHandler::ProgramHandler(HWND* _windowHandle)
{
	logicHandler = new LogicHandler();
	renderEngine = new RenderEngine(_windowHandle, logicHandler);

	logicHandler->tracePath();
	renderEngine->Update();
}


ProgramHandler::~ProgramHandler()
{
}

void ProgramHandler::PerFrame(float _deltaTime)
{
	//Updates
	if (buttonInput->GetPressed(N))
	{
		logicHandler->Regenerate();
		logicHandler->tracePath();
		renderEngine->Update();
		buttonInput->SetPressed(N, true);
	}


	//Rendering
	renderEngine->Render(_deltaTime);
}
