# Compiling the repo

## Requirements 
- GCC
- arm-none-linux-gnueabihf-gcc (for cross-compiling) **OPTIONAL**
- Cmake 
- Unix shell (OS X or Linux)

## Using the build script 
In the root directory of the repo, you should find a `build.sh` file. This guide assumes your terminal is also open in the root directory, so run the script from elsewhere at your own risk.

### Building both for PC & ARM
**NOTE** You will need arm-none-linux-gnueabihf-gcc for ARM to compile. See below for PC only instructions.

This can be done by running the build script without any arguments:
```
./build.sh
```
The outputs will be stored in the `bin/` folder. The PC & ARM version will be compiled to the `bin/x86/` and `bin/arm/` directories respectively.

### Building for PC or ARM only 
This can be done by running the build script with the `x86` argument:
```
./build.sh x86
```
The same goes for ARM, but the first argument is `arm` instead:
```
./build.sh arm
```

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