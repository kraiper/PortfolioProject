#pragma once
#include "stdafx.h"
#include "ComputeHelp.h"
#include "D3D11Timer.h"
#include "ButtonInput.h"

extern ID3D11Device* g_Device;
extern ID3D11DeviceContext* g_DeviceContext;
extern IDXGISwapChain* g_SwapChain;

extern ButtonInput* buttonInput;

using namespace DirectX;

class PathfindingRender
{
public:
	PathfindingRender(HWND* _windowHandle);
	~PathfindingRender();

	void Update(float _dt);
	void Render(float _dt);
	void UpdateMapLayout(mapData *_mapDataBuffer, std::vector<int> *_mapDataVector);
	void UpdatePath(std::vector<int2> *_pathDataVector);
	void UpdateTitle(std::string _title);
	void release();


private:
	void createCBuffers();

	template<typename T>
	void createStructuredBuffer(ComputeBuffer **_buffer, std::vector<T> *_mapData);

	void createTextureBuffer(ComputeTexture **_taxture, mapData _size);

private:
	int traversalSpeed = 10;
	int zoomSpeed = 10;

	const int changeSpeed = 5;

	int mapWidth = 1000;
	int mapHeight = 1000;

	ComputeWrap *computeWrap = NULL;

	ComputeShader *pathfinderMapTextureCS = NULL;
	ComputeShader *pathfinderPathTextureCS = NULL;
	ComputeShader *pathfinderRenderCS = NULL;
	ID3D11UnorderedAccessView *backbuffer;

	// Maptexture
	ComputeTexture *mapTexture = NULL;
	ComputeTexture *pathTexture = NULL;

	// mapData buffer
	ID3D11Buffer	*mapDataCBuffer;

	// DrawZoom buffer
	ID3D11Buffer	*drawZoomCBuffer;
	DrawZoom		drawZoomBuffer;

	// The structured buffer containing the map layout
	ComputeBuffer	*mapLayout = NULL;

	// The structured buffer containing 
	ComputeBuffer	*pathBuffer = NULL;

	HWND* windowHandle;

};

