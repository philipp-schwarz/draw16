#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawChip(int texture, int gridX, int gridY, int targetX, int targetY) {
	if (textureList[texture].alphaCache[gridY*(textureList[texture].width>>4) + gridX]) {
		d16DrawChipFine(texture, gridX, gridY, targetX, targetY);
		return;
	}
	d16DrawChipFast(texture, gridX, gridY, targetX, targetY);
}
