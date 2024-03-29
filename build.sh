#!/bin/bash

PARENT_BUILD_DIR="bin"
PC_BUILD_DIR="x86"
ARM_BUILD_DIR="arm"

CLEAN_COMMAND="clean"
TEST_COMMAND="test"
RUN_TEST_COMMAND="run_test"
EXPORT_COMMAND="export"
CONFIG_COMMAND="config"
GRAPH_COMMAND="graph"
PATCH_COMMAND="patch"
INSTALL_COMMAND="install"

CHECK_SUBMODULE_DIR="src/check/.git"
EXPORT_DIR="EXPORT"

CONFIG_PC() {
  # Check if check submodule missing
  [ ! -d "$CHECK_SUBMODULE_DIR" ] && git submodule update --init --recursive

  [ ! -d "$PC_BUILD_DIR" ] && cmake src -B $PARENT_BUILD_DIR/$PC_BUILD_DIR
}

CONFIG_ARM() {
  # Check if check submodule missing
  [ ! -d "$CHECK_SUBMODULE_DIR" ] && git submodule update --init --recursive

  [ ! -d "$ARM_BUILD_DIR" ] && cmake -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake src -B $PARENT_BUILD_DIR/$ARM_BUILD_DIR
}

CONFIG_TEST() {
  cmake -DBUILD_TESTING=ON src -B $PARENT_BUILD_DIR/$PC_BUILD_DIR
}

BUILD_PC() {
  # Apply patches first
  APPLY_PATCHES

  # Old cmake build
  #cmake --build $PARENT_BUILD_DIR/$PC_BUILD_DIR

  # Build using all cores available (faster)
  make -C $PARENT_BUILD_DIR/$PC_BUILD_DIR -j$(nproc)
}

BUILD_ARM() {
  # Apply patches first
  APPLY_PATCHES

  # cmake --build $PARENT_BUILD_DIR/$ARM_BUILD_DIR
  # Build using all cores available (faster)
  make -C $PARENT_BUILD_DIR/$ARM_BUILD_DIR -j$(nproc)
}

BUILD_BOTH() {
  BUILD_PC
  BUILD_ARM
}

MAKE_GRAPH() {
	cmake --graphviz=graph/viz.dot src -B bin/x86
	dot -Tpng graph/viz.dot -o viz.png
}

# Current only testing x86 version
RUN_TEST() {
  ctest --test-dir $PARENT_BUILD_DIR/$PC_BUILD_DIR
}

APPLY_PATCHES() {
  patch -u src/adcs/Source/CMakeLists.txt -i CMakeLists.patch
  patch -u src/adcs/Source/json-master/test/thirdparty/doctest/doctest.h -i doctest.patch
}

INSTALL_SERVICES() {
  cp service/*.service /etc/systemd/system
  systemctl enable ipc.service 
  systemctl enable dock.service
}

# Check number of arguments
if [ $# -eq 0 ]; then
  # Build for PC (default)
  BUILD_PC
elif [ "$1" == "$PC_BUILD_DIR" ]; then
  # Build (x86)
  BUILD_PC
elif [ "$1" == "$ARM_BUILD_DIR" ]; then
  # Build (arm32)
  BUILD_ARM
elif [ "$1" == "$CONFIG_COMMAND" ]; then
  if [ $# -eq 1 ]; then
    # Config (x86)
    CONFIG_PC
  elif [ "$2" == "$PC_BUILD_DIR" ]; then 
    # Config (x86)
    CONFIG_PC
  elif [ "$2" == "$ARM_BUILD_DIR" ]; then 
    # Config (arm32)
    CONFIG_ARM
  else
    # Invalid option 
    echo "$2 is not a valid option"
  fi
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
elif [ "$1" == "$GRAPH_COMMAND" ]; then
  MAKE_GRAPH
elif [ "$1" == "$PATCH_COMMAND" ]; then 
  APPLY_PATCHES
elif [ "$1" == "$INSTALL_COMMAND" ]; then 
  INSTALL_SERVICES
else
  echo "$1 is not a valid option"
fi
