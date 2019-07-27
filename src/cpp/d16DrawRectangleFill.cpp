#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawRectangleFill(uint32_t color, int x, int y, int width, int height) {
	x += 16;
	y += 16;

	if (x < 0) { width += x; x = 0; }
	if (x + width > bufferWidth) { width -= x + width - bufferWidth; }
	if (y < 0) { height += y; y = 0; }
	if (y + height > bufferHeight) { height -= y + height - bufferHeight; }

	int cx, cy;
	uint32_t* bufferData;

	for (cy=0; cy<height; cy++) {
		bufferData = (uint32_t*)buffer + x + (y+cy) * bufferWidth;
		for (cx=0; cx<width; cx++) {
			*bufferData = color;
			bufferData++;
		}
	}
}
