#!/bin/bash

PARENT_BUILD_DIR="build"
PC_BUILD_DIR="x86"
ARM_BUILD_DIR="arm"
CLEAN_COMMAND="clean"

BUILD_PC() {
  [ ! -d "$PC_BUILD_DIR" ] && cmake src -B $PARENT_BUILD_DIR/$PC_BUILD_DIR
  cmake --build $PARENT_BUILD_DIR/$PC_BUILD_DIR
}

BUILD_ARM() {
  [ ! -d "$ARM_BUILD_DIR" ] && cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake src -B $PARENT_BUILD_DIR/$ARM_BUILD_DIR
  cmake --build $PARENT_BUILD_DIR/$ARM_BUILD_DIR
}

BUILD_BOTH() {
  BUILD_PC
  BUILD_ARM
}

# Check number of arguments
if [ $# -eq 0 ]; then
  # Config and build both
  BUILD_BOTH
elif [ $# -eq 1 ]; then
  if [ "$1" == "$PC_BUILD_DIR" ]; then
    # Setup cmake (default)
    BUILD_PC
  elif [ "$1" == "$ARM_BUILD_DIR" ]; then
    # Setup cmake (arm32)
    BUILD_ARM
  elif [ "$1" == "$CLEAN_COMMAND" ]; then
    # Delete both
    [ -d "$PARENT_BUILD_DIR" ] && rm -r $PARENT_BUILD_DIR
  else
    echo "$1 is not a valid option"
  fi
else
  echo "invalid number of arguments"
fi
