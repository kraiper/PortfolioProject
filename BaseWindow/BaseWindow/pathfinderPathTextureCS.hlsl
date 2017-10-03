#include "Constants.h"

RWTexture2D<float4> output : register(u0);
RWStructuredBuffer<int2> pathData : register(u1);

Texture2D<float4> mapTexture : register(t0);

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
	int index = threadID.x + threadID.y * WIDTH;

	int2 pathDataIndex = pathData[index];

	float4 outColor = mapTexture[pathDataIndex];

	if (pathDataIndex.x != 0 && pathDataIndex.y != 0)
	{

		outColor.x = 1;
		outColor.y -= 0.5f;
		outColor.z -= 0.5f;

	}
	output[pathDataIndex] = outColor;
}