#!/bin/bash

BUILD_DIR="bin"

if [ $# -eq 1 ]; then
	if [ $1 == "clean" ]; then
		rm -r $BUILD_DIR
		echo ""
		echo "Done cleanup."
	elif [ $1 == "arm" ]; then 
		cmake -B $BUILD_DIR -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake -S .
		cmake --build $BUILD_DIR
	fi
else
	# Configure cmake 
	cmake -B $BUILD_DIR -S .

	# Build
	cmake --build $BUILD_DIR
fi
