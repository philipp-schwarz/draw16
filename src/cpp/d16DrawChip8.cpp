#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawChip8(int texture, int gridX, int gridY, int targetX, int targetY) {
	d16DrawChipFine(texture, gridX, gridY, targetX, targetY, true);
}
