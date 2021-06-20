# Using the Dock in your project

## Introduction
Given the dispersed nature of our project's source code, there needs to be a means of independently testing components which will later be unified.

Until now, no good way of making a module and testing it outside of the core software repo has been possible or convenient. 

## Purpose
This guide aims to describe the steps needed to compile, export and implement a portable version of the Dock into your project. This has module testing/development in mind especially.

## Getting started 
To start off, you'll need a copy of the core software repo and the ability to compile it. There's a detailed guide on this in the section titled **Exporting relevant libraries and headers to your project** in [COMPILING.md](/md_src_COMPILING.html).

## Exporting to your project 
Once successfully compiled and exported using the build script, you are ready to move the required libraries and headers to your project.

### Directory layout 
The EXPORT directory should have a layout like this:
```
EXPORT 
      \
      |- libLIB_DOCK.a
      |- libLIB_IPC.a
      |- include 
                 \
                 |- header1.h
                 |- header2.h
                 etc
```

Depending on the configuration of your project, it may be possible to leave everything intact and copy/paste the contents directly into your folder.

In case of a different setup, just make sure the headers can be imported into your source code AND be able to import each other. Lastly, the libraries need to be available to be linked at compile time.

## Compiling 
The simplest example which will successfully compile looks something like this:
```
gcc main.c -L. -lLIB_DOCK -lLIB_IPC -lpthread
```

### Break-down 
- `main.c` can be replaced by your source files,
- `-L.` tells the linker to look in the current directory for libraries 
- `-lLIB_DOCK -lLIB_IPC` links the required libraries to your program (must be in this order).
- `-lpthread` links the standard pthread library to your program.

## Creating & running a module on the fly
Making a portable module is slightly different (and much simpler) than a full-on module which runs on the big dock.

### The module source file 
Like usual, the module source file needs a few headers:
```C
// my_module.c 

#include "subsysmod.h"
#include "dock.h"
```

As you may notice, we now introduce the `dock.h` header, which uniquely allows us to start the dock with just one module. More on this in a minute.

### Declaring the module 
We don't actually need to create a separate header file (as usual) for the module to be declared. Simply define the start/stop functions and call the `EXPORT()` macro:
```C
// my_module.c 

#include "subsysmod.h"
#include "dock.h"

START_MODULE(my_module) {
  // do stuff
}

STOP_MODULE(my_module) {
  // stop stuff
}

EXPORT_MODULE(my_module);
```

### Running the module, solo
A new function was created to make running a single module on the dock easy. Implementing this can be as simple as creating a `main` function in the module source and calling `dock_start_solo(module)`:
```C
// my_module.c 

#include "subsysmod.h"
#include "dock.h"

START_MODULE(my_module) {
  // do stuff
}

STOP_MODULE(my_module) {
  // stop stuff
}

EXPORT_MODULE(my_module);

int main() {
  dock_start_solo(my_module);
  return 0;
}
```

That's it. If everything went well, running your program should lead to a prompt from the Dock declaring that it is running. You may quit anytime by hitting [ENTER].

## More details 
A detailed guide on making modules can be found in [MODULES.md](/md_src_MODULES.html). The only difference in the portable-module case is the use of `dock_start_solo` instead of manually adding it to the Dock.

## Different setup & issues 
If ever you are using a different configuration than shown above or run into any issues that were not covered in this guide, don't hesitate to contact me. 