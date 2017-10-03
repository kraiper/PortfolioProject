#include "RenderEngine.h"



RenderEngine::RenderEngine(HWND* _windowHandle, LogicHandler *_logicHandler)
{
	logicHandler = _logicHandler;
	pathfindingRender = new PathfindingRender(_windowHandle);
}

RenderEngine::~RenderEngine()
{

}

void RenderEngine::Update()
{
	pathfindingRender->UpdateMapLayout(logicHandler->GetMapDataBuffer(), logicHandler->GetMapDataVector());
	pathfindingRender->UpdatePath(logicHandler->GetPathVector());

	std::string title = "Generation time: " + std::to_string(logicHandler->GetTime());

	pathfindingRender->UpdateTitle(title);
}

void RenderEngine::Render(float _deltaTime)
{

	pathfindingRender->Update(_deltaTime);

	pathfindingRender->Render(_deltaTime);

	//present scene
	if (FAILED(g_SwapChain->Present(0, 0)))
		MessageBox(NULL, "Failed to present the swapchain", "RT Render Error", S_OK);
}
