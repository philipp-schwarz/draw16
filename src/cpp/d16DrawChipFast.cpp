#include "draw16.h"

void d16DrawChipFast(int texture, int gridX, int gridY, int targetX, int targetY) {

	targetX += 16;
	targetY += 16;

	if (!texture || targetX < 0 || targetY < 0 || targetX > screenWidthRange || targetY > screenHeightRange)
		return;

	int i;

	int bufferOffset = targetX + targetY * bufferWidth;
	int textureOffset = (gridX<<4) + (gridY<<4) * textureList[texture].width;
	
	for (i=0; i<16; i++) {
		memcpy((uint32_t*)buffer + bufferOffset, (uint32_t*)textureList[texture].data + textureOffset, 64);
		bufferOffset += bufferWidth;
		textureOffset += textureList[texture].width;
	}

}