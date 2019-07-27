#include "draw16.h"

// Screen
SDL_Surface *screen;
int screenSize = 0;
int screenWidth = 0;
int screenHeight = 0;
int screenWidthRange = 0;
int screenHeightRange = 0;
int screenStride = 0;
int screenStride16 = 0;

// Pixel buffer
uint8_t* buffer;
int bufferSize = 0;
int bufferWidth = 0;
int bufferHeight = 0;
int bufferStride = 0;
int bufferStride16 = 0;

// Texture
Texture* textureList = new Texture[TEXTURE_LIMIT];
int textureCount = 1;
