#!/bin/bash

PARENT_BUILD_DIR="bin"
PC_BUILD_DIR="x86"
ARM_BUILD_DIR="arm"
CLEAN_COMMAND="clean"
TEST_COMMAND="test"
RUN_TEST_COMMAND="run_test"
CHECK_SUBMODULE_DIR="src/check/.git"
EXPORT_COMMAND="export"
EXPORT_DIR="EXPORT"

CONFIG_PC() {
  [ ! -d "$PC_BUILD_DIR" ] && cmake src -B $PARENT_BUILD_DIR/$PC_BUILD_DIR
}

CONFIG_ARM() {
  [ ! -d "$ARM_BUILD_DIR" ] && cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake src -B $PARENT_BUILD_DIR/$ARM_BUILD_DIR
}

CONFIG_TEST() {
  cmake -DBUILD_TESTING=ON src -B $PARENT_BUILD_DIR/$PC_BUILD_DIR
}

BUILD_PC() {
  cmake --build $PARENT_BUILD_DIR/$PC_BUILD_DIR
}

BUILD_ARM() {
  cmake --build $PARENT_BUILD_DIR/$ARM_BUILD_DIR
}

BUILD_BOTH() {
  BUILD_PC
  BUILD_ARM
}

# Current only testing x86 version
RUN_TEST() {
  ctest --test-dir $PARENT_BUILD_DIR/$PC_BUILD_DIR
}

# Check if check submodule missing
[ ! -d "$CHECK_SUBMODULE_DIR" ] && git submodule update --init --recursive

# Check number of arguments
if [ $# -eq 0 ]; then
  # Config and build both
  CONFIG_PC && BUILD_PC
  CONFIG_ARM && BUILD_ARM
elif [ "$1" == "$PC_BUILD_DIR" ]; then
  CONFIG_PC && BUILD_PC
elif [ "$1" == "$ARM_BUILD_DIR" ]; then
  # Setup cmake (arm32)
  CONFIG_ARM && BUILD_ARM
elif [ "$1" == "$CLEAN_COMMAND" ]; then
  # Delete both
  [ -d "$PARENT_BUILD_DIR" ] && rm -r $PARENT_BUILD_DIR
  [ -d "$EXPORT_DIR" ]  && rm -r $EXPORT_DIR
elif [ "$1" == "$TEST_COMMAND" ]; then
  CONFIG_TEST
  BUILD_PC
elif [ "$1" == "$RUN_TEST_COMMAND" ]; then
  # Note: currently only testing x86 version
  CONFIG_TEST
  BUILD_PC
  RUN_TEST

elif [ "$1" == "$EXPORT_COMMAND" ]; then 
  CONFIG_PC && BUILD_PC
  mkdir $EXPORT_DIR
  cp $PARENT_BUILD_DIR/$PC_BUILD_DIR/libLIB_DOCK.a $EXPORT_DIR
  cp $PARENT_BUILD_DIR/$PC_BUILD_DIR/libLIB_IPC.a $EXPORT_DIR
  mkdir $EXPORT_DIR/include 
  # cp src/api/libipc/inc/subsysmod.h $EXPORT_DIR/include
  # cp src/api/libipc/inc/modutil.h $EXPORT_DIR/include
  # cp src/api/libipc/inc/ipc_settings.h $EXPORT_DIR/include
  cp src/api/libipc/inc/*.h $EXPORT_DIR/include
  # cp src/api/client_api/inc/client_api.h $EXPORT_DIR/include
  cp src/api/client_api/inc/*.h $EXPORT_DIR/include
  # cp src/libdock/inc/dock.h $EXPORT_DIR/include 
  # cp src/libdock/inc/dock_settings.h $EXPORT_DIR/include 
  cp src/libdock/inc/*.h $EXPORT_DIR/include 
  echo ""
  echo "#######################################################"
  echo "# Export complete.                                    #"
  echo "# The required libraries can be found in EXPORT/.     #"
  echo "# The relevant headers were put in EXPORT/include/.   #"
  echo "# Enjoy.                                              #"
  echo "#######################################################"
else
  echo "$1 is not a valid option"
fi
