#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <emscripten.h>

#define abs(x) ((x)<0 ? -(x) : (x))

/*****************************************************************************
	Config
*/

#define TEXTURE_LIMIT 5000

// Screen
extern SDL_Surface *screen;
extern int screenSize;
extern int screenWidth;
extern int screenHeight;
extern int screenWidthRange;
extern int screenHeightRange;
extern int screenStride;
extern int screenStride16;

// Pixel buffer
extern uint8_t* buffer;
extern int bufferSize;
extern int bufferWidth;
extern int bufferHeight;
extern int bufferStride;
extern int bufferStride16;

// Texture
struct Texture {
	unsigned int width;
	unsigned int height;
	unsigned int stride;
	unsigned int stride16;
	uint8_t* data;
	bool* alphaCache;
};

extern Texture* textureList;
extern int textureCount;

// Functions
extern "C" void EMSCRIPTEN_KEEPALIVE d16Clear();
extern void d16DrawChipFast(int texture, int gridX, int gridY, int targetX, int targetY);
extern void d16DrawChipFine(int texture, int gridX, int gridY, int targetX, int targetY, bool px8=false);
extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawChip(int texture, int gridX, int gridY, int targetX, int targetY);
extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawChip8(int texture, int gridX, int gridY, int targetX, int targetY);
extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawArea(
	int texture,
	int targetX, int targetY,
	int targetWidth=0, int targetHeight=0,
	int gridX=0, int gridY=0,
	int sourceWidth=0, int sourceHeight=0
);
extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawText8(int texture, int font, uint32_t color, char* string, int targetX, int targetY);
extern "C" int EMSCRIPTEN_KEEPALIVE d16DrawText8Length(int texture, int font, char* string);
extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawPixel(uint32_t color, int x, int y);
extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawRectangleFill(uint32_t color, int x, int y, int width, int height);
extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawRectangle(uint32_t color, int x, int y, int width, int height);
extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawEllipseFill(uint32_t color, int xm, int ym, int a, int b);
extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawEllipse(uint32_t color, int xm, int ym, int a, int b);
extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawLine(uint32_t color, int x, int y, int x2, int y2);
extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawShiftLineH(int y, int offset);

extern "C" void EMSCRIPTEN_KEEPALIVE d16Flush();
extern "C" void EMSCRIPTEN_KEEPALIVE d16Init(int width, int height);
extern "C" void EMSCRIPTEN_KEEPALIVE d16Resize(int width, int height);
extern "C" int EMSCRIPTEN_KEEPALIVE d16TextureLoad(int width, int height, uint8_t *data);
