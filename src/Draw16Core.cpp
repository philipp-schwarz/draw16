#include <stdio.h>
#include <string.h>
#include <SDL/SDL.h>
#include <emscripten.h>

#define abs(x) ((x)<0 ? -(x) : (x))

/*****************************************************************************
	Config
*/

#define TEXTURE_LIMIT 5000

/*****************************************************************************
	Variables
*/

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
struct Texture {
	unsigned int width;
	unsigned int height;
	unsigned int stride;
	unsigned int stride16;
	uint8_t* data;
	bool* alphaCache;
};

Texture* textureList = new Texture[TEXTURE_LIMIT];
int textureCount = 1;


/*****************************************************************************
	Functions
*/

extern "C" void d16Clear() {

	int i;
	for (i=0; i<bufferSize; i++) {
		buffer[i++] = 0;
		buffer[i++] = 0;
		buffer[i++] = 0;
		buffer[i] = 255;
	}

}

void d16DrawFast(int texture, int gridX, int gridY, int targetX, int targetY) {

	targetX += 16;
	targetY += 16;

	if (!texture || targetX < 0 || targetY < 0 || targetX > screenWidthRange || targetY > screenHeightRange)
		return;

	int i;

	int bufferOffset = targetX + targetY * bufferWidth;
	int textureOffset = (gridX<<4) + (gridY<<4) * textureList[texture].width;
	
	for (i=0; i<16; i++) {
		memcpy((uint32_t*)buffer + bufferOffset, (uint32_t*)textureList[texture].data + textureOffset, 64);
		bufferOffset += bufferWidth;
		textureOffset += textureList[texture].width;
	}

}

void d16DrawAlpha(int texture, int gridX, int gridY, int targetX, int targetY, bool px8=false) {

	targetX += 16;
	targetY += 16;

	int factor = px8 ? 8 : 16;

	if (!texture || targetX < 0 || targetY < 0 || targetX > screenWidthRange || targetY > screenHeightRange)
		return;

	int textureStride16 = textureList[texture].stride16;
	int i, j;

	uint8_t* bufferData = (uint8_t*)buffer;
	uint8_t* textureData = (uint8_t*)textureList[texture].data;

	bufferData += (targetX + targetY * bufferWidth) * 4;
	textureData += (gridX * factor + gridY * factor * textureList[texture].width) * 4;

	for (i=0; i<factor; i++) {
		for (j=0; j<factor; j++) {

			// switch alpha channel
			switch (textureData[3]) {

				// full visible
				case 255:
				case 254:
					*bufferData = *textureData;
					bufferData++;
					textureData++;
					*bufferData = *textureData;
					bufferData++;
					textureData++;
					*bufferData = *textureData;
					bufferData += 2;
					textureData += 2;
					break;

				// invisible / maximum transparency
				case 0:
				case 1:
					bufferData += 4;
					textureData += 4;
					break;

				// 50% aplha
				case 127:
				case 128:
					*bufferData = ((*textureData >> 1) + (*bufferData >> 1));
					bufferData++;
					textureData++;
					*bufferData = ((*textureData >> 1) + (*bufferData >> 1));
					bufferData++;
					textureData++;
					*bufferData = ((*textureData >> 1) + (*bufferData >> 1));
					bufferData += 2;
					textureData += 2;
					break;

				// 25% aplha
				case 63:
				case 64:
					*bufferData = ((*textureData >> 2) + (*bufferData >> 2) + (*bufferData >> 1));
					bufferData++;
					textureData++;
					*bufferData = ((*textureData >> 2) + (*bufferData >> 2) + (*bufferData >> 1));
					bufferData++;
					textureData++;
					*bufferData = ((*textureData >> 2) + (*bufferData >> 2) + (*bufferData >> 1));
					bufferData += 2;
					textureData += 2;
					break;

				// 75% aplha
				case 191:
				case 192:
					*bufferData = ((*bufferData >> 2) + (*textureData >> 2) + (*textureData >> 1));
					bufferData++;
					textureData++;
					*bufferData = ((*bufferData >> 2) + (*textureData >> 2) + (*textureData >> 1));
					bufferData++;
					textureData++;
					*bufferData = ((*bufferData >> 2) + (*textureData >> 2) + (*textureData >> 1));
					bufferData += 2;
					textureData += 2;
					break;

				// fine transparency
				default:
					uint8_t alpha = textureData[3];
					*bufferData = (*textureData * alpha + *bufferData * (255 - alpha)) / 255;
					bufferData++;
					textureData++;
					*bufferData = (*textureData * alpha + *bufferData * (255 - alpha)) / 255;
					bufferData++;
					textureData++;
					*bufferData = (*textureData * alpha + *bufferData * (255 - alpha)) / 255;
					bufferData += 2;
					textureData += 2;
			}
		}
		bufferData += bufferStride16;
		textureData += textureStride16;
		if (px8) {
			bufferData += 4*8;
			textureData += 4*8;
		}
	}
}

extern "C" void d16DrawChip(int texture, int gridX, int gridY, int targetX, int targetY) {
	if (textureList[texture].alphaCache[gridY*(textureList[texture].width>>4) + gridX]) {
		d16DrawAlpha(texture, gridX, gridY, targetX, targetY);
		return;
	}
	d16DrawFast(texture, gridX, gridY, targetX, targetY);
}

extern "C" void d16DrawChip8(int texture, int gridX, int gridY, int targetX, int targetY) {
	d16DrawAlpha(texture, gridX, gridY, targetX, targetY, true);
}

extern "C" void d16DrawArea(
	int texture,
	int targetX, int targetY,
	int targetWidth=0, int targetHeight=0,
	int gridX=0, int gridY=0,
	int sourceWidth=0, int sourceHeight=0
) {
	if (!sourceWidth || !sourceHeight) {
		sourceWidth = textureList[texture].width>>4;
		sourceHeight = textureList[texture].height>>4;
	}
	if (!targetWidth)
		targetWidth = sourceWidth;
	if (!targetHeight)
		targetHeight = sourceHeight;
	int x, y;
	int cy, cx;
	for (y=0; y<targetHeight; y++) {
		for (x=0; x<targetWidth; x++) {
			// TODO: PERFORMANCE
			d16DrawChip(
				texture,
				gridX + (x % sourceWidth),
				gridY + (y % sourceHeight),
				targetX + (x<<4),
				targetY + (y<<4)
			);
		}
	}
}

extern "C" void d16DrawText8(int texture, int font, uint32_t color, char* string, int targetX, int targetY) {

	font *= 2;
	targetX += 16;
	targetY += 16;

	int second = 0;

	int r = color & 0xff;
	int g = (color >> 8) & 0xff;
	int b = (color >> 16) & 0xff;

	if (!texture || targetY < 0 || targetY > screenHeightRange)
		return;

	int textureStride16 = textureList[texture].stride16;
	int o, i, j, c, w;

	uint8_t* bufferData;
	uint8_t* textureData;

	for (o=0; o<100; o++) {
		c = string[o];
		if (c == 0) return;
		if (c == 126 && !second) {
			second = 1;
		}
		else if (c >= 32 && c <= 126) {
			c -= 32;

			if (targetX > screenWidthRange)
				return;

			bufferData = (uint8_t*)buffer + (targetX + targetY * bufferWidth) * 4;
			textureData = (uint8_t*)textureList[texture].data + (c * 8 + (font + second) * 8 * textureList[texture].width) * 4;

			w = 0;
			for (i=0; i<8; i++) {
				for (j=0; j<8; j++) {

					// switch alpha channel
					switch (textureData[3]) {

						// full visible
						case 255:
						case 254:
							if (j > w)
								w = j;

							if (targetX > 0) {
								*bufferData = *textureData & r;
								bufferData++;
								textureData++;
								*bufferData = *textureData & g;
								bufferData++;
								textureData++;
								*bufferData = *textureData & b;
								bufferData += 2;
								textureData += 2;
							}
							else {
								bufferData += 4;
								textureData += 4;								
							}
							break;

						// invisible / maximum transparency
						default:
							bufferData += 4;
							textureData += 4;
							break;
					}
				}
				bufferData += bufferStride16 + 4*8;
				textureData += textureStride16 + 4*8;
			}

			targetX += w+2;
			if (second)
				second = 0;
		}
	}
}

extern "C" void d16DrawPixel(uint32_t color, int x, int y) {
	x += 16;
	y += 16;

	if (x < 0 || y < 0 || x > bufferWidth || y > bufferHeight)
		return;

	color = color | 0xff000000;

	uint32_t* bufferData = (uint32_t*)buffer + (x + y * bufferWidth);
	*bufferData = color;
}

extern "C" void d16DrawRectangleFill(uint32_t color, int x, int y, int width, int height) {
	x += 16;
	y += 16;

	if (x < 0) { width += x; x = 0; }
	if (x + width > bufferWidth) { width -= x + width - bufferWidth; }
	if (y < 0) { height += y; y = 0; }
	if (y + height > bufferHeight) { height -= y + height - bufferHeight; }

	int cx, cy;
	uint32_t* bufferData;

	for (cy=0; cy<height; cy++) {
		bufferData = (uint32_t*)buffer + x + (y+cy) * bufferWidth;
		for (cx=0; cx<width; cx++) {
			*bufferData = color;
			bufferData++;
		}
	}
}

extern "C" void d16DrawRectangle(uint32_t color, int x, int y, int width, int height) {
	d16DrawRectangleFill(color, x, y, width, 1);
	d16DrawRectangleFill(color, x, y, 1, height);
	d16DrawRectangleFill(color, x, y+height-1, width, 1);
	d16DrawRectangleFill(color, x+width-1, y, 1, height);
}

extern "C" void d16DrawEllipseFill(uint32_t color, int xm, int ym, int a, int b) {
	int ox = 1 - (a % 2);
	int oy = 1 - (b % 2);
	xm += a/2;
	ym += b/2;
	a /= 2;
	b /= 2;

	int dx = 0, dy = b;
	long a2 = a*a, b2 = b*b;
	long err = b2-(2*b-1)*a2, e2;

	do {
		d16DrawRectangleFill(color, xm-dx, ym+dy-oy, 1+2*dx-ox, 1);
		d16DrawRectangleFill(color, xm-dx, ym-dy, 1+2*dx-ox, 1);

		e2 = 2*err;
		if (e2 <  (2*dx+1)*b2) { dx++; err += (2*dx+1)*b2; }
		if (e2 > -(2*dy-1)*a2) { dy--; err -= (2*dy-1)*a2; }

	} while (dy >= 0);

	while (dx++ < a) {
		d16DrawRectangleFill(color, xm-dx, ym, 1+2*dx-ox, 1);
	}
}

extern "C" void d16DrawEllipse(uint32_t color, int xm, int ym, int a, int b) {
	int ox = 1 - (a % 2);
	int oy = 1 - (b % 2);
	xm += a/2;
	ym += b/2;
	a /= 2;
	b /= 2;

	int dx = 0, dy = b;
	long a2 = a*a, b2 = b*b;
	long err = b2-(2*b-1)*a2, e2;

	do {
		d16DrawPixel(color, xm+dx-ox, ym+dy-oy);
		d16DrawPixel(color, xm-dx, ym+dy-oy);
		d16DrawPixel(color, xm-dx, ym-dy);
		d16DrawPixel(color, xm+dx-ox, ym-dy);

		e2 = 2*err;
		if (e2 <  (2*dx+1)*b2) { dx++; err += (2*dx+1)*b2; }
		if (e2 > -(2*dy-1)*a2) { dy--; err -= (2*dy-1)*a2; }

	} while (dy >= 0);

	while (dx++ < a) {
		d16DrawPixel(color, xm+dx-ox, ym);
		d16DrawPixel(color, xm-dx, ym);
	}
}

extern "C" void d16DrawLine(uint32_t color, int x, int y, int x2, int y2) {
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

extern "C" void d16DrawShiftLineH(int y, int offset) {
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

extern "C" void d16Flush() {

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

extern "C" void d16Init(int width, int height) {

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

extern "C" int d16TextureLoad(int width, int height, uint8_t *data) {

	if (width % 16 || height % 16 || width <= 0 || height <= 0)
		return -1;

	int i;
	int x, y, cx, cy;
	int length = width * height * 4;
	int id = textureCount++;
	bool alpha;

	textureList[id].width = width;
	textureList[id].stride = width * 4;
	textureList[id].stride16 = (width - 16) * 4;
	textureList[id].height = height;
	textureList[id].data = new uint8_t[length];
	textureList[id].alphaCache = new bool[width * height];

	for (i = 0; i < length; i++) {
		textureList[id].data[i] = data[i];
	}

	i = 0;
	for (y=0; y<height/16; y++) {
		for (x=0; x<width/16; x++) {
			alpha = false;
			for (cy=0; cy<16; cy++) {
				for (cx=0; cx<16; cx++) {
					if (textureList[id].data[(y*16+cy)*width*4 + (x*16+cx)*4 + 3] != 255) {
						alpha = true;
					}
				}
			}
			textureList[id].alphaCache[i++] = alpha;
		}
	}

	return id;
}
