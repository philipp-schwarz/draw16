#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawText8(int texture, int font, uint32_t color, char* string, int targetX, int targetY) {

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
