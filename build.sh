#!/bin/bash
cd "$(dirname "$0")"

echo "Building Draw16..."
echo "Make sure 'emcc' and 'uglifyjs' are installed and in your \$PATH"

if [ -f dist/Draw16.js ] ; then
	rm dist/Draw16.js
	echo "Cleaning: Draw16.js"
fi

if [ -f dist/Draw16.min.js ] ; then
	rm dist/Draw16.min.js
	echo "Cleaning: Draw16.min.js"
fi

echo "Making: Draw16.js"
emcc src/cpp/*.cpp \
	--pre-js src/js/Draw16CorePre.js \
	--post-js src/js/Draw16CorePost.js \
	--post-js src/js/Draw16.js \
	-o dist/Draw16.js \
	-s EXPORT_NAME="'Draw16Core'" \
	-s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']"

echo "Making: Draw16.min.js"
uglifyjs dist/Draw16.js --compress --output dist/Draw16.min.js

echo "Done!"
