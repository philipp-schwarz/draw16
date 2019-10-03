#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawRectangleFillAlpha(uint32_t color, int alpha, int x, int y, int width, int height) {
	x += 16;
	y += 16;

	if (x < 0) { width += x; x = 0; }
	if (x + width > bufferWidth) { width -= x + width - bufferWidth; }
	if (y < 0) { height += y; y = 0; }
	if (y + height > bufferHeight) { height -= y + height - bufferHeight; }

	int cx, cy;
	uint8_t* bufferData;

	int r = color & 0xff;
	int g = (color & 0xff00) >> 8;
	int b = (color & 0xff0000) >> 16;

	for (cy=0; cy<height; cy++) {
		bufferData = (uint8_t*)buffer + (x + (y+cy) * bufferWidth) * 4;
		for (cx=0; cx<width; cx++) {
			*bufferData = (r * alpha + *bufferData * (255 - alpha)) / 255;
			bufferData++;
			*bufferData = (g * alpha + *bufferData * (255 - alpha)) / 255;
			bufferData++;
			*bufferData = (b * alpha + *bufferData * (255 - alpha)) / 255;
			bufferData += 2;
		}
	}
}
