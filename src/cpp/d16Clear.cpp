#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16Clear() {

	int i;
	for (i=0; i<bufferSize; i++) {
		buffer[i++] = 0;
		buffer[i++] = 0;
		buffer[i++] = 0;
		buffer[i] = 255;
	}

}
