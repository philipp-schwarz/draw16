#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16Init(int width, int height) {

	int i, x, y;
	int c = 0;

	// Clear
	for (i=0; i<TEXTURE_LIMIT; i++) {
		textureList[i].width = 0;
		textureList[i].height = 0;
	}

	// Init screen
	screenWidth = width;
	screenHeight = height;
	screenWidthRange = width+16;
	screenHeightRange = height+16;
	screenStride = width * 4;
	screenStride16 = (width-16) * 4;
	screenSize = screenStride * screenHeight;

	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);


	// Init buffer
	bufferWidth = width + 32;
	bufferHeight = height + 32;
	bufferStride = bufferWidth * 4;
	bufferStride16 = (bufferWidth-16) * 4;
	bufferSize = bufferStride * bufferHeight;

	buffer = new uint8_t[bufferWidth * bufferHeight * 4];


	// Init screen
	if (SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);

	d16Clear();

	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);

	SDL_Flip(screen);

	if (SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);

}