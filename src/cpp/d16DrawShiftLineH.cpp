#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawShiftLineH(int y, int offset) {
	y += 16;

	if (offset < -16 || offset > 16 || offset == 0)
		return;

	if (y < 0 || y >= bufferHeight)
		return;

	int i;
	uint32_t* bufferData = (uint32_t*)buffer + y * bufferWidth + 16;
	
	if (offset > 0) {
		for (i=0; i<screenWidth; i++) {
			bufferData[i] = bufferData[i + offset];
		}
	}
	else {
		for (i=screenWidth-1; i>=0; i--) {
			bufferData[i] = bufferData[i + offset];
		}
	}
}
