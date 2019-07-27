#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawArea(
	int texture,
	int targetX, int targetY,
	int targetWidth, int targetHeight,
	int gridX, int gridY,
	int sourceWidth, int sourceHeight
) {
	if (!sourceWidth || !sourceHeight) {
		sourceWidth = textureList[texture].width>>4;
		sourceHeight = textureList[texture].height>>4;
	}
	if (!targetWidth)
		targetWidth = sourceWidth;
	if (!targetHeight)
		targetHeight = sourceHeight;
	int x, y;
	int cy, cx;
	for (y=0; y<targetHeight; y++) {
		for (x=0; x<targetWidth; x++) {
			// TODO: PERFORMANCE
			d16DrawChip(
				texture,
				gridX + (x % sourceWidth),
				gridY + (y % sourceHeight),
				targetX + (x<<4),
				targetY + (y<<4)
			);
		}
	}
}
