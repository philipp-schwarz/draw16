#include "draw16.h"

extern "C" int EMSCRIPTEN_KEEPALIVE d16DrawText8Length(int texture, int font, char* string) {
	int length = 0;

	font *= 2;
	int second = 0;
	if (!texture)
		return 0;

	int textureStride16 = textureList[texture].stride16;
	int o, i, j, c, w;

	uint8_t* textureData;

	for (o=0; o<100; o++) {
		c = string[o];
		if (c == 0) break;
		if (c == 126 && !second) {
			second = 1;
		}
		else if (c >= 32 && c <= 126) {
			c -= 32;

			textureData = (uint8_t*)textureList[texture].data + (c * 8 + (font + second) * 8 * textureList[texture].width) * 4;

			w = 0;
			for (i=0; i<8; i++) {
				for (j=0; j<8; j++) {
					switch (textureData[3]) {
						case 255:
						case 254:
							if (j > w)
								w = j;
							break;
					}
					textureData += 4;								
				}
				textureData += textureStride16 + 4*8;
			}

			length += w+2;
			if (second)
				second = 0;
		}
	}

	length -= 2;
	if (length < 0)
		return 0;
	return length;
}
