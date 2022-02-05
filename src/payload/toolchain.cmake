set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# set(CMAKE_SYSROOT /usr/local/lib/arm-none-linux-gnueabihf/arm-none-linux-gnueabihf)

set(tools /bin/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/bin)
set(CMAKE_C_COMPILER /bin/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /bin/gcc-arm-10.2-2020.11-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")