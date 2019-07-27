#include "draw16.h"

extern "C" int EMSCRIPTEN_KEEPALIVE d16TextureLoad(int width, int height, uint8_t *data) {

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
