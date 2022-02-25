# Compiling the repo

## Requirements 
- Core software repo
- GCC
- arm-none-linux-gnueabihf-gcc (for cross-compiling) **OPTIONAL**
- Cmake 
- Unix shell (OS X or Linux)

## Getting the core software repo 
The core software repo may be found in [DalhousieSpaceSystemsLab/CubeSat-Core](https://github.com/DalhousieSpaceSystemsLab/CubeSat-Core). 

To get started, go into your programming folder of choice and clone the repo:
```
git clone https://github.com/DalhousieSpaceSystemsLab/CubeSat-Core
```

Once cloned, you may enter the root directory using `cd CubeSat-Core`. 

You are ready to start compiling.

## Using the build script 
In the root directory of the repo, you should find a `build.sh` file. This guide assumes your terminal is also open in the root directory, so run the script from elsewhere at your own risk.

### Configuring the build files (PC or ARM)
Before any build commands can be run, the build files must first be configured.

This can be achieved for PC or ARM using the following command:
```
./build.sh config <arch>
```

Where `<arch>` is either `x86` or `arm` depending on what your target platform is.

### Building for PC or ARM only 
Once configured, you may build the project the same `<arch>` argument:
```
./build.sh <arch>
```
Where `<arch>` is either `x86` or `arm` depending on what your target platform is.

NOTE: Usually you only need to configure your build files once, but when troubleshooting it may be useful to reconfigure them.

### Exporting relevant libraries and headers to your project
This can be done using the `export` argument in the build script:
```
./build.sh export
```
The core software will automatically be compiled and a folder called 'EXPORT' will automatically be populated with the libraries and headers you'll need to develop/test modules independently.

### Cleaning up
If you'd like to clean up all of the build objects/cache quickly, use:
```
./build.sh clean
```
This will automatically remove the binaries, object files and CMake cache.

## Compiling manually 
Sometimes you may want to do this to be able to manually set some parameters or just try it yourself. 
CMake is always run in 2 steps: first the build folder is configured, then it is compiled. 

### Configuring the build folder for PC 
This can be done using the following command:
```
cmake src -B <name of build folder>
```

### Configuring the build folder for ARM
This can be done using the following command:
```
cmake -DCMAKE_TOOLCHAIN_FILE=<path to toolchain.cmake> src -B <name of build folder>
```
More information about cross-compiling with cmake can be found here: [cmake-toolchains](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html)

### Compiling 
Once succesfully configured, you may compile the configured build folder using:
```
cmake --build <build folder>
```

That's it. Happy coding