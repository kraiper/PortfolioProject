#include "PathfindingRender.h"
#include <ctime>


PathfindingRender::PathfindingRender(HWND* _windowHandle)
{
	HRESULT hr = S_OK;
	windowHandle = _windowHandle;

	// initiate computeWrap
	computeWrap = new ComputeWrap(g_Device, g_DeviceContext);

	// Get the backbuffer as a texture
	ID3D11Texture2D* pBackBuffer;
	hr = g_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		MessageBox(NULL, "failed getting the backbuffer", "RTRenderDX11 Error", S_OK);

	// create shader unordered access view on back buffer for compute shader to write into texture
	hr = g_Device->CreateUnorderedAccessView(pBackBuffer, NULL, &backbuffer);
	if (FAILED(hr))
		MessageBox(NULL, "failed assigning the backbuffer", "RTRenderDX11 Error", S_OK);

	// initiate the computeshader
	pathfinderRenderCS = computeWrap->CreateComputeShader("pathfinderRenderCS");
	pathfinderMapTextureCS = computeWrap->CreateComputeShader("pathfinderMapTextureCS");
	pathfinderPathTextureCS = computeWrap->CreateComputeShader("pathfinderPathTextureCS");

	// create the map constant buffer
	createCBuffers();

	// Randomize the map

	srand(time(0));

	drawZoomBuffer.drawFromX = 0;
	drawZoomBuffer.drawFromY = 0;
	drawZoomBuffer.drawWidth = 100;
	drawZoomBuffer.drawHeight = 100;

	g_DeviceContext->UpdateSubresource(drawZoomCBuffer, 0, NULL, &drawZoomBuffer, 0, 0);

}


PathfindingRender::~PathfindingRender()
{
}

void PathfindingRender::Update(float _dt)
{
	bool updateBuffer = false;

	// Traversal
	if (buttonInput->GetPressed(R))
	{
		traversalSpeed += changeSpeed;
		buttonInput->SetPressed(R, true);
	}
	if (buttonInput->GetPressed(F))
	{
		traversalSpeed -= changeSpeed;
		buttonInput->SetPressed(F, true);
	}

	if (traversalSpeed < changeSpeed)
	{
		traversalSpeed = changeSpeed;
	}

	if (buttonInput->GetPressed(W))
	{
		drawZoomBuffer.drawFromY -= traversalSpeed;
		updateBuffer = true;
		buttonInput->SetPressed(W, true);
	}
	if (buttonInput->GetPressed(S))
	{
		drawZoomBuffer.drawFromY += traversalSpeed;
		updateBuffer = true;
		buttonInput->SetPressed(S, true);
	}
	if (buttonInput->GetPressed(A))
	{
		drawZoomBuffer.drawFromX -= traversalSpeed;
		updateBuffer = true;
		buttonInput->SetPressed(A, true);
	}
	if (buttonInput->GetPressed(D))
	{
		drawZoomBuffer.drawFromX += traversalSpeed;
		updateBuffer = true;
		buttonInput->SetPressed(D, true);
	}

	// Zoom
	if (buttonInput->GetPressed(Y))
	{
		zoomSpeed += changeSpeed;
		buttonInput->SetPressed(Y, true);
	}
	if (buttonInput->GetPressed(H))
	{
		zoomSpeed -= changeSpeed;
		buttonInput->SetPressed(H, true);
	}

	if (zoomSpeed < changeSpeed)
	{
		zoomSpeed = 1;
	}

	if (buttonInput->GetPressed(T))
	{
		drawZoomBuffer.drawHeight += zoomSpeed;
		drawZoomBuffer.drawWidth += zoomSpeed;
		drawZoomBuffer.drawFromX -= zoomSpeed / 2;
		drawZoomBuffer.drawFromY -= zoomSpeed / 2;
		updateBuffer = true;
		buttonInput->SetPressed(T, true);
	}
	if (buttonInput->GetPressed(G))
	{
		drawZoomBuffer.drawHeight -= zoomSpeed;
		drawZoomBuffer.drawWidth -= zoomSpeed;
		drawZoomBuffer.drawFromX += zoomSpeed / 2;
		drawZoomBuffer.drawFromY += zoomSpeed / 2;
		updateBuffer = true;
		buttonInput->SetPressed(G, true);
	}

	// Update constant buffer
	if (updateBuffer)
	{
		g_DeviceContext->UpdateSubresource(drawZoomCBuffer, 0, NULL, &drawZoomBuffer, 0, 0);
	}


}

void PathfindingRender::Render(float _dt)
{
	// First fill the full map texture with data

	//set shader
	pathfinderMapTextureCS->Set();

	//set the texture as output
	ID3D11UnorderedAccessView* uavTexture[] = { mapTexture->GetUnorderedAccessView() };
	g_DeviceContext->CSSetUnorderedAccessViews(0, 1, uavTexture, NULL);
	
	//set map data layout buffer
	ID3D11UnorderedAccessView* uav[] = { mapLayout->GetUnorderedAccessView(), pathTexture->GetUnorderedAccessView() };
	g_DeviceContext->CSSetUnorderedAccessViews(1, 2, uav, NULL);

	//dispatch

	int threadMultiplierX = (mapWidth * 3) / (NROFTHREADSWIDTH * CORETHREADSWIDTH);
	int threadMultiplierY = (mapHeight * 3) / (NROFTHREADSHEIGHT * CORETHREADSHEIGHT);

	threadMultiplierX = threadMultiplierX < 1 ? 1 : threadMultiplierX;
	threadMultiplierY = threadMultiplierY < 1 ? 1 : threadMultiplierY;

	g_DeviceContext->Dispatch(NROFTHREADSWIDTH * threadMultiplierX, NROFTHREADSHEIGHT * threadMultiplierX, 1);

	//unbind resource views
	ID3D11UnorderedAccessView* nulluav[] = { NULL, NULL, NULL };
	g_DeviceContext->CSSetUnorderedAccessViews(0, 3, nulluav, NULL);

	//unset shader
	pathfinderMapTextureCS->Unset();

	// Draw the path to the map texture ------------------------------------------------------------------------------
	pathfinderPathTextureCS->Set();

	// Set the path output texture
	ID3D11UnorderedAccessView* uavPathTexture[] = { pathTexture->GetUnorderedAccessView(), pathBuffer->GetUnorderedAccessView() };
	g_DeviceContext->CSSetUnorderedAccessViews(0, 2, uavPathTexture, NULL);

	// Set the map texture
	ID3D11ShaderResourceView *srvMapTexture[] = { mapTexture->GetResourceView() };
	g_DeviceContext->CSSetShaderResources(0, 1, srvMapTexture);

	//dispatch
	g_DeviceContext->Dispatch(NROFTHREADSWIDTH, NROFTHREADSHEIGHT, 1);

	//unbind resource views
	g_DeviceContext->CSSetUnorderedAccessViews(0, 2, nulluav, NULL);

	ID3D11ShaderResourceView *nullsrv[] = { NULL };
	g_DeviceContext->CSSetShaderResources(0, 1, nullsrv);

	//unset shader
	pathfinderPathTextureCS->Unset();

	// Transfer the selected bit of the map to the backbuffer---------------------------------------------------------------------

	// set shader
	pathfinderRenderCS->Set();

	// set the backbuffer as output
	g_DeviceContext->CSSetUnorderedAccessViews(0, 1, &backbuffer, NULL);

	// set the texture
	ID3D11ShaderResourceView *srvPathTexture[] = { pathTexture->GetResourceView() };
	g_DeviceContext->CSSetShaderResources(0, 1, srvPathTexture);

	//dispatch
	g_DeviceContext->Dispatch(NROFTHREADSWIDTH, NROFTHREADSHEIGHT, 1);

	//unbind resource views
	g_DeviceContext->CSSetUnorderedAccessViews(0, 2, nulluav, NULL);

	g_DeviceContext->CSSetShaderResources(0, 1, nullsrv);

	// unset shader
	pathfinderRenderCS->Unset();
}

void PathfindingRender::UpdateMapLayout(mapData * _mapDataBuffer, std::vector<int>* _mapDataVector)
{
	int camStartX;
	int camStartY;
	int camEndX;
	int camEndY;

	if (_mapDataBuffer->startX < _mapDataBuffer->endX)
	{
		camStartX = _mapDataBuffer->startX;
		camEndX = _mapDataBuffer->endX;
	}
	else
	{
		camStartX = _mapDataBuffer->endX;
		camEndX = _mapDataBuffer->startX;
	}

	if (_mapDataBuffer->startY < _mapDataBuffer->endY)
	{
		camStartY = _mapDataBuffer->startY;
		camEndY = _mapDataBuffer->endY;
	}
	else
	{
		camStartY = _mapDataBuffer->endY;
		camEndY = _mapDataBuffer->startY;
	}

	drawZoomBuffer.drawFromX = camStartX - 10;
	drawZoomBuffer.drawFromY = camStartY - 10;
	int size = (camEndX - camStartX) > (camEndY - camStartY) ? camEndX - camStartX : camEndY - camStartY;


	drawZoomBuffer.drawHeight = size + 20;
	drawZoomBuffer.drawWidth = size + 20;

	mapWidth = _mapDataBuffer->mapWidth;
	mapHeight = _mapDataBuffer->mapHeight;

	g_DeviceContext->UpdateSubresource(drawZoomCBuffer, 0, NULL, &drawZoomBuffer, 0, 0);

	g_DeviceContext->UpdateSubresource(mapDataCBuffer, 0, NULL, _mapDataBuffer, 0, 0);
	
	createTextureBuffer(&mapTexture, *_mapDataBuffer);
	createTextureBuffer(&pathTexture, *_mapDataBuffer);

	createStructuredBuffer(&mapLayout, _mapDataVector);

}

void PathfindingRender::UpdatePath(std::vector<int2>* _pathDataVector)
{
	createStructuredBuffer(&pathBuffer, _pathDataVector);
}

void PathfindingRender::UpdateTitle(std::string _title)
{
	SetWindowText(*windowHandle, _title.c_str());
}

void PathfindingRender::release()
{
	SAFE_RELEASE(mapDataCBuffer);
	SAFE_RELEASE(backbuffer);

	SAFE_DELETE(pathfinderRenderCS);
	SAFE_DELETE(computeWrap);
}

void PathfindingRender::createCBuffers()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.Usage = D3D11_USAGE_DEFAULT;
	// CPU writable, should be updated per frame
	cbDesc.CPUAccessFlags = 0;
	cbDesc.MiscFlags = 0;

	///mapData cbuffer
	if (sizeof(mapData) % 16 > 0)
	{
		cbDesc.ByteWidth = (int)((sizeof(mapData) / 16) + 1) * 16;
	}
	else
	{
		cbDesc.ByteWidth = sizeof(mapData);
	}

	hr = g_Device->CreateBuffer(&cbDesc, NULL, &mapDataCBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed Making Constant Buffer: cBuffer", "Create Buffer", MB_OK);
	}
	g_DeviceContext->CSSetConstantBuffers(0, 1, &mapDataCBuffer);

	///drawZoom cbuffer
	if (sizeof(DrawZoom) % 16 > 0)
	{
		cbDesc.ByteWidth = (int)((sizeof(DrawZoom) / 16) + 1) * 16;
	}
	else
	{
		cbDesc.ByteWidth = sizeof(DrawZoom);
	}

	hr = g_Device->CreateBuffer(&cbDesc, NULL, &drawZoomCBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, "Failed Making Constant Buffer: cBuffer", "Create Buffer", MB_OK);
	}
	g_DeviceContext->CSSetConstantBuffers(1, 1, &drawZoomCBuffer);
}

void PathfindingRender::createTextureBuffer(ComputeTexture ** _buffer, mapData _size)
{
	/*if (*_buffer != NULL)
	{
		SAFE_RELEASE(*_buffer);
	}*/


	*_buffer = computeWrap->CreateTexture(
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		_size.mapWidth,
		_size.mapHeight,
		0,
		NULL,
		false,
		"Map Texture");
}


template<typename T>
inline void PathfindingRender::createStructuredBuffer(ComputeBuffer ** _buffer, std::vector<T>* _mapData)
{
	if (*_buffer != NULL)
	{
		SAFE_RELEASE(*_buffer);
	}

	*_buffer = computeWrap->CreateBuffer(STRUCTURED_BUFFER,
		sizeof(T),
		_mapData->size(),
		false,
		true,
		false,
		_mapData->data(),
		false,
		"Structed Buffer: MapData");
}
