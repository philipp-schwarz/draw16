Draw16 = {};

Draw16.canvas = null;

Draw16.texture = {};
Draw16.ready = false;
Draw16.step = 0;

Draw16.fullscreen = false;
Draw16.fullscreenFactorX = 0;
Draw16.fullscreenFactorY = 0;
Draw16.fullscreenOffsetX = 0;
Draw16.fullscreenOffsetY = 0;
Draw16.width = 0;
Draw16.height = 0;

Draw16.fitMode = 0;
Draw16._fitMode = -1;
Draw16.FIT_CONTAIN = 0;
Draw16.FIT_COVER = 1;
Draw16.FIT_STRETCH = 2;
Draw16.FIT_VERTICAL = 3;
Draw16.FIT_HORIZONTAL = 4;

Draw16.load = 0;
Draw16.fps = 30;
Draw16.fpsLast = 0;
Draw16.fpsMeasureStart = 0;
Draw16.fpsMeasureTime = 0;
Draw16.fpsMeasureSteps = 0;

Draw16.mouseX = 0;
Draw16.mouseY = 0;
Draw16.mouseHide = false;
Draw16._mouseHide = false;

window.setInterval(function() {
	Draw16.fpsLast = Draw16.fpsMeasureSteps;
	Draw16.fpsMeasureSteps = 0;

	Draw16.load = Math.round(Draw16.fpsMeasureTime / 10);
	Draw16.fpsMeasureTime = 0;
}, 1000);

Draw16.color = function(r, g, b) {
	return r + (g << 8) + (b << 16)
}

Draw16.init = function(width, height) {
	Draw16.width = width;
	Draw16.height = height;

	Draw16.canvas = document.getElementById('draw16Canvas');

	Draw16.canvas.addEventListener("webglcontextlost", function(e) {
		window.location.href = window.location.href;
		e.preventDefault();
	}, false);

	Draw16Core._d16Init(width, height);

	Draw16.init = function() {}
	Draw16.ready = true;

	window.requestAnimationFrame(Draw16.onStepWrapper);
}

Draw16.resize = function(width, height) {
	width = width|0;
	height = height|0;
	if (!width)
		width = Draw16.width;
	if (!height)
		height = Draw16.height;

	if (Draw16.width == width && Draw16.height == height) {
		return false;
	}

	Draw16.width = width;
	Draw16.height = height;

	Draw16.canvas = document.getElementById('draw16Canvas');

	Draw16Core._d16Resize(width, height);
	Draw16._fitMode = -1;

	return true;
}

Draw16.installFullscreen = function() {

	Draw16.fullscreen = true;

	document.body.style.backgroundColor = '#000';
	document.body.style.margin = '0';
	document.body.style.padding = '0';
	document.body.style.overflow = 'hidden';

	var viewport = document.createElement('meta');
	viewport.name = 'viewport';
	viewport.content = 'width=device-width,initial-scale=1.0,maximum-scale=1.0,user-scalable=no';
	document.getElementsByTagName('head')[0].appendChild(viewport);

	document.addEventListener('touchstart', function (event) { event.preventDefault(); }, false);
	document.addEventListener('touchmove', function (event) { event.preventDefault(); }, false);
	document.addEventListener('touchend', function (event) { event.preventDefault(); }, false);
	document.addEventListener('mousedown', function (event) { event.preventDefault(); }, false);
	document.addEventListener('mouseup', function (event) { event.preventDefault(); }, false);
	document.addEventListener('contextmenu', function (event) { event.preventDefault(); }, false);

	var canvas = Draw16.canvas;

	canvas.style.position = 'fixed';
	canvas.style.left = 'calc(50% - '+Math.floor(Draw16.width/2)+'px)';
	canvas.style.top = 'calc(50% - '+Math.floor(Draw16.height/2)+'px)';
	canvas.style.transform = 'scale('+(window.innerHeight / Draw16.height)+')';

	canvas.style.imageRendering = 'optimizeSpeed';
	canvas.style.imageRendering = '-moz-crisp-edges';
	canvas.style.imageRendering = '-o-crisp-edges';
	canvas.style.imageRendering = '-webkit-optimize-contrast';
	canvas.style.imageRendering = 'pixelated';
	canvas.style.imageRendering = 'optimize-contrast';
	canvas.style.msInterpolationMode = 'nearest-neighbor'; // you found the special snowflake


}

Draw16._updateCanvasSize = function() {

	if (!Draw16.fullscreen)
		return;

	var canvas = Draw16.canvas;

	if (Draw16.fitMode == Draw16.FIT_VERTICAL) {
		Draw16.fullscreenFactorX = Draw16.fullscreenFactorY = window.innerHeight / Draw16.height;
		canvas.style.transform = 'scale('+(window.innerHeight / Draw16.height)+')';
	}
	
	else if (Draw16.fitMode == Draw16.FIT_HORIZONTAL) {
		Draw16.fullscreenFactorX = Draw16.fullscreenFactorY = window.innerWidth / Draw16.width;
		canvas.style.transform = 'scale('+(window.innerWidth / Draw16.width)+')';
	}

	else if (Draw16.fitMode == Draw16.FIT_STRETCH) {
		Draw16.fullscreenFactorX = window.innerWidth / Draw16.width;
		Draw16.fullscreenFactorY = window.innerHeight / Draw16.height;
		canvas.style.transform = 'scale('+(window.innerWidth / Draw16.width)+', '+(window.innerHeight / Draw16.height)+')';
	}

	else if (
		Draw16.fitMode == Draw16.FIT_CONTAIN
		|| Draw16.fitMode == Draw16.FIT_COVER
	) {
		var wr = window.innerWidth / window.innerHeight;
		var cr = Draw16.width / Draw16.height;

		if (
			(Draw16.fitMode == Draw16.FIT_CONTAIN && wr > cr)
			|| (Draw16.fitMode == Draw16.FIT_COVER && wr < cr)
		) {
			Draw16.fullscreenFactorX = Draw16.fullscreenFactorY = window.innerHeight / Draw16.height;
			canvas.style.transform = 'scale('+(window.innerHeight / Draw16.height)+')';
		}
		else {
			Draw16.fullscreenFactorX = Draw16.fullscreenFactorY = window.innerWidth / Draw16.width;
			canvas.style.transform = 'scale('+(window.innerWidth / Draw16.width)+')';
		}
	}

	Draw16.fullscreenOffsetX = Math.round((window.innerWidth - Draw16.width*Draw16.fullscreenFactorX) / 2);
	Draw16.fullscreenOffsetY = Math.round((window.innerHeight - Draw16.height*Draw16.fullscreenFactorY) / 2);

	canvas.style.left = 'calc(50% - '+Math.floor(Draw16.width/2)+'px)';
	canvas.style.top = 'calc(50% - '+Math.floor(Draw16.height/2)+'px)';
}

Draw16.loadTexture = function(image) {
	var width = image.width;
	var height = image.height;

	var canvas = document.createElement('canvas');
	canvas.width = width;
	canvas.height = height;

	var context = canvas.getContext('2d');
	context.drawImage(image, 0, 0, width, height);

	var imageData = context.getImageData(0, 0, width, height);
	var data = imageData.data;

	var pointerToBuffer = Draw16Core._malloc(data.length);
	var buffer = new Uint8Array(Draw16Core.HEAPU8.buffer, pointerToBuffer, data.length);
	buffer.set(new Uint8Array(data.buffer));

	var value = Draw16Core.ccall('d16TextureLoad', 'number',['number','number','number'], [width, height, buffer.byteOffset]);
	Draw16Core._free(buffer.byteOffset);

	return value;
}

Draw16.loadTextureAsync = function(name, src, cb) {
	Draw16.texture[name] = 0;
	var img = new Image();
	img.onload = function() {
		Draw16.texture[name] = Draw16.loadTexture(img);
		if (cb)
			cb(name, img, Draw16.texture[name]);
	}
	img.src = src;
}

Draw16.flush = function() { Draw16Core._d16Flush(); }
Draw16.clear = function() { Draw16Core._d16Clear(); }

Draw16.drawChip = Draw16Core._d16DrawChip; // texture, gridX, gridY, targetX, targetY
Draw16.drawChip8 = Draw16Core._d16DrawChip8; // texture, gridX, gridY, targetX, targetY
Draw16.drawArea = Draw16Core._d16DrawArea; // texture, targetX, targetY, targetWidth, targetHeight, gridX, gridY, sourceWidth, sourceHeight
Draw16.drawText8 = Draw16Core.cwrap('d16DrawText8', null, ['number', 'number', 'number', 'string', 'number', 'number']);
Draw16.drawText8Length = Draw16Core.cwrap('d16DrawText8Length', 'number', ['number', 'number', 'string']);

Draw16.drawPixel = Draw16Core._d16DrawPixel; // color, targetX, targetY
Draw16.drawLine = Draw16Core._d16DrawLine; // color, targetX, targetY, targetX2, targetY2
Draw16.drawEllipse = Draw16Core._d16DrawEllipse; // color, targetX, targetY, width, height
Draw16.drawEllipseFill = Draw16Core._d16DrawEllipseFill; // color, targetX, targetY, width, height
Draw16.drawRectangle = Draw16Core._d16DrawRectangle; // color, targetX, targetY, width, height
Draw16.drawRectangleFill = Draw16Core._d16DrawRectangleFill; // color, targetX, targetY, width, height

Draw16.d16DrawShiftLineH = Draw16Core._d16DrawShiftLineH; // y, offset

Draw16.onStep = function() {}
Draw16.onStepLastTime = 0;

Draw16.onStepWrapper = function() {
	var currentTime = Date.now();
	var delta = 1000 / Draw16.fps;
	if (currentTime - Draw16.onStepLastTime > delta) {
		Draw16.onStepLastTime += delta;
		if (currentTime - Draw16.onStepLastTime > delta * 3) {
			Draw16.onStepLastTime = currentTime - delta * 3;
		}

		Draw16.fpsMeasureStart = window.performance.now();

		Draw16.clear();
		Draw16.onStep();
		Draw16.flush();

		Draw16.fpsMeasureTime += window.performance.now() - Draw16.fpsMeasureStart;
		Draw16.fpsMeasureSteps++;

		if (Draw16.mouseHide != Draw16._mouseHide) {
			Draw16._mouseHide = Draw16.mouseHide;

			var canvas = document.getElementById('draw16Canvas');
			canvas.style.cursor = Draw16.mouseHide ? 'none' : 'auto';
		}

		if (Draw16.fitMode != Draw16._fitMode) {
			Draw16._fitMode = Draw16.fitMode;

			Draw16._updateCanvasSize();
		}

		Draw16.step++;
	}

	window.requestAnimationFrame(Draw16.onStepWrapper);
}

Draw16.screenToPixelPosition = function(x, y) {
	x = x / Draw16.fullscreenFactorX - Draw16.fullscreenOffsetX / Draw16.fullscreenFactorX;
	y = y / Draw16.fullscreenFactorY - Draw16.fullscreenOffsetY / Draw16.fullscreenFactorY;
	return { x: Math.floor(x), y: Math.floor(y) };
}

document.addEventListener('mousemove', function(event) {
	if (Draw16.fullscreen) {

		var f = window.innerHeight / Draw16.height;

		var x = event.clientX;
		var y = event.clientY;

		x = x / f;
		y = y / f;

		x = x - (window.innerWidth/f - Draw16.width) / 2;

		Draw16.mouseX = Math.floor(x);
		Draw16.mouseY = Math.floor(y);
	}
}, false);

window.addEventListener('resize', function() {
	Draw16._fitMode = -1;
});