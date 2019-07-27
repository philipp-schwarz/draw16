if (typeof Draw16Core == 'undefined') {
	var Draw16Core = Module;
}

Draw16Core.canvas = document.getElementById('draw16Canvas');
if (!Draw16Core.canvas) {
	Draw16Core.canvas = document.createElement('canvas');
	Draw16Core.canvas.id = 'draw16Canvas';
	document.body.appendChild(Draw16Core.canvas);
}

Draw16Core.canvas.addEventListener("webglcontextlost", function(e) {
	window.location.href = window.location.href;
	e.preventDefault();
}, false);

if (!Draw16Core.print) {
	Draw16Core.print = console.log;
}

if (!Draw16Core.printErr) {
	Draw16Core.printErr = console.error;
}