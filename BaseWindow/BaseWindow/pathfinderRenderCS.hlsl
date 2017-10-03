#include "Constants.h"

RWTexture2D<float4> output : register(u0);

Texture2D<float4> pathTexture : register(t0);

cbuffer mapData : register(b0)
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

cbuffer mapData : register(b1)
{
	int drawFromX;
	int drawFromY;
	int drawWidth;
	int drawHeight;
};

[numthreads(CORETHREADSWIDTH, CORETHREADSHEIGHT, 1)]
void main(uint3 threadID : SV_DispatchThreadID)
{
	int2 textureIndex;

	int widthIndex = floor(threadID.x / (WIDTH / drawWidth));

	int heightIndex = floor(threadID.y / (HEIGHT / drawHeight));

	textureIndex.x = widthIndex + drawFromX;
	textureIndex.y = heightIndex + drawFromY;

	output[threadID.xy] = saturate(pathTexture[textureIndex.xy]);

}