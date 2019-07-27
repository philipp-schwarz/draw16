#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawLine(uint32_t color, int x, int y, int x2, int y2) {
	x += 16;
	y += 16;
	x2 += 16;
	y2 += 16;

	int dx = abs(x2 - x);
	int sx = x < x2 ? 1 : -1;
	int dy = -abs(y2 - y);
	int sy = y < y2 ? 1 : -1;
	int err = dx + dy;
	int e2;

	uint32_t* bufferData;

	color = color | 0xff000000;

	while (true) {
		if (x >= 0 && y >= 0 && x < bufferWidth && y < bufferHeight) {
			bufferData = (uint32_t*)buffer + x + y * bufferWidth;
			*bufferData = color;
		}

		if (x == x2 && y == y2)
			break;
		
		e2 = 2 * err;
		
		if (e2 > dy) {
			err += dy;
			x += sx;
		}
		
		if (e2 < dx) {
			err += dx;
			y += sy;
		}
	}
}