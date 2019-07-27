#include "draw16.h"

extern "C" void EMSCRIPTEN_KEEPALIVE d16DrawEllipseFill(uint32_t color, int xm, int ym, int a, int b) {
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
