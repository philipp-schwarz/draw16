#include "draw16.h"

void d16DrawChipFine(int texture, int gridX, int gridY, int targetX, int targetY, bool px8) {

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
					bufferData += 4;
					textureData += 4;
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
