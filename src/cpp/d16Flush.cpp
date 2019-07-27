#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16Flush() {

	uint32_t* bufferData = (uint32_t*)buffer;
	uint32_t* screenData = (uint32_t*)screen->pixels;

	bufferData += 16 * bufferWidth + 16;

	int i;
	for (i=0; i<screenHeight; i++) {
		memcpy(screenData, bufferData, screenStride);
		screenData += screenWidth;
		bufferData += bufferWidth;
	}

	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);

	SDL_Flip(screen);

	if (SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);

}
