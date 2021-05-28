# Exporting to your project 
There are some headers/libraries in the core software repo you want to test or work with in your own repo. 

This guide will explain how to export/integrate each part into your project.

## Using the export script 
In the root directory of the repo, you can find the `export_client.sh` script. 

This script will automatically compile the repo for PC and extract the relevant header files and libraries into the `EXPORT_CLIENT/` folder.

It can be run using:
```
./export_client.sh
```
Once the export script runs successfully, you may copy `libCLIENT_API.a` and `include/client_api.h` to your project directory.

## Linking to your project
In order for the client API to work, you must link the `libCLIENT_API.a` library to your project.

### Manually linking 
If you are manually compiling using `gcc` or something, you just need to include `-lCLIENT_API` for the linker to include the library. This will work as long as you've copied the library from `EXPORT_CLIENT` to your working directory, as shown earlier.

### Linking in CMake 
Linking a library to an executable CMake can be done with the following line in your `CMakeLists.txt`:
```CMake 
TARGET_LINK_LIBRARIES(${TARGET_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/libCLIENT_API.a")
```
This must be called after you declare the executable with `add_executable(...)`. Once this is added, cmake will take care of the rest.

Like earlier, the library must be found in the working directory. In this case, the absolute path given the cmake in the example leads to the `src` folder. Feel free to set the absolute path of your choosing; but I believe cmake is happiest with absolute paths. 

## Including the header 
Once the required libraries are linked, all that is left to do is to `#include include/client_api.h` into your source code. 

If you have a different directory layout for header files (like all source files and headers in the same folder, for example), feel free to change the path of the header file in `#include`. 

