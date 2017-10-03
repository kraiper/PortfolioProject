#include "Constants.h"

RWTexture2D<float4> output : register(u0);

RWStructuredBuffer<int> mapLayout : register(u1);

RWTexture2D<float4> output2 : register(u2);

cbuffer mapData : register(c0)
{
	int mapWidth;
	int mapHeight;
	int startX;
	int startY;
	int endX;
	int endY;
	int padding1;
	int padding2;
};

[numthreads(CORETHREADSWIDTH, CORETHREADSHEIGHT, 1)]
void main(uint3 threadID : SV_DispatchThreadID)
{

	float mapColor = mapLayout[threadID.x + threadID.y * mapHeight];

	float startPos = 0;

	if (threadID.x == startX)
	{
		if (threadID.y == startY)
		{
			startPos = 1;
		}
	}

	float endPos = 0;

	if (threadID.x == endX)
	{
		if (threadID.y == endY)
		{
			endPos = 1;
		}
	}

	float4 outColor = float4(mapColor - startPos - endPos, mapColor - endPos, mapColor - startPos, 1);

	output[threadID.xy] = saturate(outColor);
	output2[threadID.xy] = saturate(outColor);
}