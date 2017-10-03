#include "LogicHandler.h"



LogicHandler::LogicHandler()
{

	// Randomize the map

	srand(time(0));
	Regenerate();

}


LogicHandler::~LogicHandler()
{
	mapDataVector.clear();
	mapDataVector.resize(0);
}

void LogicHandler::Regenerate()
{
	mapDataBuffer.mapWidth = 100 * (std::rand() % 100 + 1);
	mapDataBuffer.mapHeight = mapDataBuffer.mapWidth;
	mapDataBuffer.endX = (std::rand() % mapDataBuffer.mapWidth);
	mapDataBuffer.endY = (std::rand() % mapDataBuffer.mapHeight);
	mapDataBuffer.startX = (std::rand() % mapDataBuffer.mapWidth);
	mapDataBuffer.startY = (std::rand() % mapDataBuffer.mapHeight);
	mapDataBuffer.padding1 = 0;
	mapDataBuffer.padding1 = 0;

	mapDataVector.clear();
	for (int i = 0; i < (mapDataBuffer.mapWidth * mapDataBuffer.mapHeight);i++)
	{
		if ((std::rand() % 2) == 0)
		{
			mapDataVector.push_back((std::rand() % 2));
		}
		else
		{
			mapDataVector.push_back(1);
		}
	}

	while (mapDataVector.at(mapDataBuffer.startX + mapDataBuffer.startY * mapDataBuffer.mapWidth) == 0)
	{
		mapDataBuffer.startX = (std::rand() % mapDataBuffer.mapWidth);
		mapDataBuffer.startY = (std::rand() % mapDataBuffer.mapHeight);
	}

	while (mapDataVector.at(mapDataBuffer.endX + mapDataBuffer.endY * mapDataBuffer.mapWidth) == 0)
	{
		mapDataBuffer.endX = (std::rand() % mapDataBuffer.mapWidth);
		mapDataBuffer.endY = (std::rand() % mapDataBuffer.mapHeight);
	}
}

void LogicHandler::tracePath()
{
	unsigned char *pMap = new unsigned char[mapDataVector.size()];

	for (int i = 0; i < mapDataVector.size(); i++)
	{
		pMap[i] = mapDataVector.at(i);
	}

	start = std::clock();

	int result = pathfinderLogic.FindPath(mapDataBuffer.startX, mapDataBuffer.startY, mapDataBuffer.endX, mapDataBuffer.endY, pMap, mapDataBuffer.mapWidth, mapDataBuffer.mapHeight, pOutBuffer, nOutBufferSize);

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	pathDataVector.clear();
	for (int i = 0; i < result; i++)
	{
		int2 temp = int2(int(pOutBuffer[i] % mapDataBuffer.mapWidth), int(pOutBuffer[i] / mapDataBuffer.mapWidth));
		pathDataVector.push_back(temp);
	}

	if (result == -1)
	{
		duration = -duration;
	}

	delete(pMap);

}


