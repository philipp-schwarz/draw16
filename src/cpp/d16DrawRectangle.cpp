#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawRectangle(uint32_t color, int x, int y, int width, int height) {
	d16DrawRectangleFill(color, x, y, width, 1);
	d16DrawRectangleFill(color, x, y, 1, height);
	d16DrawRectangleFill(color, x, y+height-1, width, 1);
	d16DrawRectangleFill(color, x+width-1, y, 1, height);
}
