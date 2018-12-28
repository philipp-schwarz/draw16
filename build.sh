#!/bin/bash
cd "$(dirname "$0")"

echo "Building Draw16..."
echo "Make sure 'emcc' and 'uglifyjs' are installed and in your \$PATH"

if [ -f lib/Draw16.js ] ; then
	rm lib/Draw16.js
	echo "Cleaning: Draw16.js"
fi

if [ -f lib/Draw16.min.js ] ; then
	rm lib/Draw16.min.js
	echo "Cleaning: Draw16.min.js"
fi

echo "Making: Draw16.js"
emcc src/Draw16Core.cpp \
	--pre-js src/Draw16CorePre.js \
	--post-js src/Draw16CorePost.js \
	--post-js src/Draw16.js \
	-o lib/Draw16.js \
	-s EXPORT_NAME="'Draw16Core'" \
	-s EXPORTED_FUNCTIONS="['_d16Init', '_d16Clear', '_d16DrawChip', '_d16DrawChip8', '_d16DrawArea', '_d16DrawText8', '_d16DrawPixel', '_d16DrawLine', '_d16DrawRectangle', '_d16DrawRectangleFill', '_d16DrawEllipse', '_d16DrawEllipseFill', '_d16DrawShiftLineH', '_d16Flush', '_d16TextureLoad']" \
	-s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']"

echo "Making: Draw16.min.js"
uglifyjs lib/Draw16.js --compress --output lib/Draw16.min.js

echo "Done!"
