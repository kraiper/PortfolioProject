#pragma once
#include "stdafx.h"
#include "PathfinderLogic.h"
#include <ctime>

class LogicHandler
{
public:
	LogicHandler();
	~LogicHandler();

	mapData* GetMapDataBuffer() { return &mapDataBuffer; };
	std::vector<int>* GetMapDataVector() { return &mapDataVector; };
	std::vector<int2>* GetPathVector() { return &pathDataVector; };
	double GetTime() { return duration; };

	void Regenerate();
	void tracePath();

private:

	mapData			mapDataBuffer;
	std::vector<int> mapDataVector;
	std::vector<int2> pathDataVector;

	PathfinderLogic pathfinderLogic;

	int nOutBufferSize = 10000;
	int *pOutBuffer = new int[nOutBufferSize];

	std::clock_t start;
	double duration;

};

