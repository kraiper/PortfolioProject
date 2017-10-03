#pragma once
#include <stdlib.h>
#include <vector>
#include <ctime>

struct SearchNode
{
	int posX;
	int posY;
	int costToPos;
	int estimatedCostToGoal;
	std::vector<int> path;
};

class PathfinderLogic
{
public:
	int FindPath(const int nStartX, const int nStartY,
		const int nTargetX, const int nTargetY,
		const unsigned char* pMap, const int nMapWidth, const int nMapHeight,
		int* pOutBuffer, const int nOutBufferSize) 
	{
		double start = std::clock();

		unsigned char *traversalMap = new unsigned char[nMapWidth * nMapHeight];

		for (int i = 0; i < nMapWidth * nMapHeight; i++)
		{
			traversalMap[i] = pMap[i];
		}
		traversalMap[nStartX + nStartY * nMapWidth] = 2;

		std::vector<SearchNode> pathList;

		SearchNode node;
		node.posX = nStartX;
		node.posY = nStartY;
		node.costToPos = 0;
		node.estimatedCostToGoal = abs(node.posX - nTargetX) + abs(node.posY - nTargetY);

		pathList.push_back(node);

		if (node.estimatedCostToGoal > nOutBufferSize)
		{
			return -1;
		}

		int rightIndex = 1;
		int leftIndex = -1;
		int uppIndex = -nMapWidth;
		int downIndex = nMapWidth;

		int calculationNode = 0;
		int currentIndex = nStartX + nStartY * nMapWidth;

		
		while (nTargetX != node.posX || nTargetY != node.posY)
		{

			// Right
			if (node.posX + 1 < nMapWidth)
			{
				if (traversalMap[currentIndex + rightIndex] == 1)
				{
					traversalMap[currentIndex + rightIndex] = 2;

					node = pathList.at(calculationNode);

					node.posX++;
					node.costToPos++;
					node.estimatedCostToGoal = abs(node.posX - nTargetX) + abs(node.posY - nTargetY);
					node.path.push_back(currentIndex + rightIndex);

					pathList.push_back(node);
				}
			}

			// Left
			if (node.posX - 1 > 0)
			{
				if (traversalMap[currentIndex + leftIndex] == 1)
				{
					traversalMap[currentIndex + leftIndex] = 2;

					node = pathList.at(calculationNode);

					node.posX--;
					node.costToPos++;
					node.estimatedCostToGoal = abs(node.posX - nTargetX) + abs(node.posY - nTargetY);
					node.path.push_back(currentIndex + leftIndex);

					pathList.push_back(node);
				}
			}

			// Upp
			if (node.posY - 1 > 0)
			{
				if (traversalMap[currentIndex + uppIndex] == 1)
				{
					traversalMap[currentIndex + uppIndex] = 2;

					node = pathList.at(calculationNode);

					node.posY--;
					node.costToPos++;
					node.estimatedCostToGoal = abs(node.posX - nTargetX) + abs(node.posY - nTargetY);
					node.path.push_back(currentIndex + uppIndex);

					pathList.push_back(node);
				}
			}

			// Down
			if (node.posY + 1 < nMapHeight)
			{
				if (traversalMap[currentIndex + downIndex] == 1)
				{
					traversalMap[currentIndex + downIndex] = 2;

					node = pathList.at(calculationNode);

					node.posY++;
					node.costToPos++;
					node.estimatedCostToGoal = abs(node.posX - nTargetX) + abs(node.posY - nTargetY);
					node.path.push_back(currentIndex + downIndex);

					pathList.push_back(node);
				}
			}

			pathList.erase(pathList.begin() + calculationNode);
			calculationNode = 0;

			if (pathList.size() == 0)
			{
				return -1;
			}

			// Select next work node
			for (int i = 0; i < pathList.size(); i++)
			{
				// Find the best node to calculate
				if ((pathList.at(i).costToPos / 2 + pathList.at(i).estimatedCostToGoal)< (pathList.at(calculationNode).costToPos / 2 + pathList.at(calculationNode).estimatedCostToGoal))
				{
					calculationNode = i;
				}
			}
			node = pathList.at(calculationNode);
			currentIndex = node.posX + node.posY * nMapWidth;

			if ((std::clock() - start) / (double)CLOCKS_PER_SEC > 4.95)
			{
				return -1;
			}
		}

		if (node.costToPos > nOutBufferSize)
		{
			return -1;
		}

		for (int i = 0; i < node.path.size(); i++)
		{
			pOutBuffer[i] = node.path.at(i);
		}


		// Return
		return node.costToPos;
	};

};


