#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawPixel(uint32_t color, int x, int y) {
	x += 16;
	y += 16;

	if (x < 0 || y < 0 || x > bufferWidth || y > bufferHeight)
		return;

	color = color | 0xff000000;

	uint32_t* bufferData = (uint32_t*)buffer + (x + y * bufferWidth);
	*bufferData = color;
}
