# Guide on making modules

## Intro
Modules are an important part of the core software, as they allow us to easily containerize and expand the available features of the software base.

Additionally, all subsystem software may be abstracted as a module, making it an ideal way to package every process which needs to run on the OBC (with a few exceptions).

## What you need 
To get started making modules, you need a working version of the core software repo. A guide on how to do this may be found in [COMPILING.md](...).

## Finding the modules directory 
The core software repo is rather large, so it can be a bit of a mess to find your way around. 

In this case, we're concerned with the modules folder, which can be found here:
```
CubeSat-Core
\
  src
      \
        modules
                \
                 |- inc
                 |- src
```

I'll draw your attention to the `inc` and `src` folders inside of `modules`. 

All of the module header files (.h) must be placed in the `inc` folder, which stands for include. All of the headers placed in this folder will be public, so other parts of the code can see them as well.

All of the module source files (.c) must be placed in the `src` folder.

### What you should find 
Taking a look at the existing modules should help give you an idea of what making a module will look like. 

Looking into the `inc` and `src` folders, you should see something like:
```
modules 
       |\
       |  inc
       |      \
       |       |-  command_module.h
       |       |-  filesystem_module.h
       |       |-  gps_module.h
       |       |-  missions_module.h
       |       |-  payload_module.h
       |       |-  rf_module.h
       |        
        \
          src
              \
               |- command_module.c
               |- filesystem_module.c
               |- gps_module.c
               |- mission_module.c
               |- payload_module.c
               |- rf_module.c

```

As you can see, every `<something>_module.h` header in `inc` has a corresponding `<something>_module.c` source file in `src`.

This is done intentionally, because the source files are where all of the module code is stored. Each header file, then, is used to make the container the dock uses to load the module.

### Visualizing the Dock 
So one way to visualize this could be: 
```
               --------
      ------  |  Dock  |  ------
     |         --------         |
     |            ||            |
 ----------   ----------   ----------
| module 1 | | module 2 | | module 3 |
|  header  | |  header  | |  header  |
 ----------   ----------   ----------
     |            ||            |
 ----------   ----------   ----------
| module 1 | | module 2 | | module 3 |
|  source  | |  source  | |  source  |
 ----------   ----------   ----------
 ```

## Creating your first module 
First you'll need to create 2 files: `my_module.h` in the `inc` folder and `my_module.c` in the `src` folder.

### The module header file 
This is a good place to start since the existence of the module is first declared in the header.

#### The header guard 
It is good practice in C to always place a header guard in your header files. An example of one would be to do like so:

```C
// my_module.h 

#ifndef CUBESAT_CORE_MODULES_MY_MODULE_H
#define CUBESAT_CORE_MODULES_MY_MODULE_H

// your code goes here...

#endif
```

This way, everything between the `#define` and `#endif` will be protected against double includes.

You can pretty much replace `CUBESAT_CORE_MODULES_MY_MODULE_H` with anything you want, as long as you're sure it is unique. 

#### Including the necessary headers 
Next, we must include the headers needed to make a module. There is only one, called `subsysmod.h` which is shorthand for subsystem module.

Your module header should look something like this once it has been included:

```C
// my_module.h 

#ifndef CUBESAT_CORE_MODULES_MY_MODULE_H
#define CUBESAT_CORE_MODULES_MY_MODULE_H

#include "subsysmod.h"

#endif
```

#### Declaring the module 
Now we're ready to declare the existence of the new module. To do this, we use one of the handy macros `NEW_DOCK_MODULE(name)`. 

In this case, as the module is called `my_module`, we will declare it like so:

```C
// my_module.h 

#ifndef CUBESAT_CORE_MODULES_MY_MODULE_H
#define CUBESAT_CORE_MODULES_MY_MODULE_H

#include "subsysmod.h"

NEW_DOCK_MODULE(my_module);

#endif
```

Now, it is possible for the Dock to import your module. But before anything will work, we need to write some code for the module first.

### The module source code 
Now that the header file is ready, we may move on to the source file. Go ahead and create `my_module.c` in the `src` folder.

#### Including your module header 
The only header that is needed to be included is your module header, even if you are going to use more libraries or headers than just your own. 

This is because all of the headers you'd like to include must go in your module header, not your module source. This way, when the Dock imports your module header, everything it needs is included and in one place.

So, your source file should look something like this:
```C
// my_module.c 

#include "my_module.h"
```

#### Creating the start/stop functions 
Every module has a start and a stop function. These functions are called by the Dock when the module needs to be started or stopped.

You can create these using the `START_MODULE(name) {}` and `STOP_MODULE(name) {}` macros. 

Once they are added, your source code should look something like this:
```C
// my_module.c 

#include "my_module.h"

START_MODULE(my_module) {

}

STOP_MODULE(my_module) {

}
```

#### Exporting the module 
This is the last step. 

Once both start/stop methods have been defined, the last thing to do is export them to the header using the `EXPORT_MODULE(name)` macro.

This is just to make sure that the source file is linked to the header file. The export statement must also go at the end of your source file. 

```C
// my_module.c 

#include "my_module.h"

START_MODULE(my_module) {

}

STOP_MODULE(my_module) {

}

EXPORT_MODULE(my_module);
```

And that's it, you've successfully made a module.

## Adding the module to the Dock
Your module must be added to the Dock's source code before it will automatically load it on start. 

### Locating the Dock source code 
I will move your attention elsewhere in the core software, to the Dock source code:
```
CubeSat-Core
            \
              src
                  \
                    dock
                        \
                         |- inc 
                         |- src 
                                \
                                 |- main.c 
                                 |- dock.c
```

Here you can safely ignore the `dock.c` file. The place your module needs to be imported is in the `main.c` file.

### Importing your module 
Inside the dock's `main.c`, near the beginning, you'll find a long list of includes for all of the modules. It should look something like:
```C
// Subsystem modules
#include "command_module.h"
#include "gps_module.h"
#include "mission_module.h"
#include "payload_module.h"
#include "rf_module.h"
```

So all you need to do is add an entry for your module's header, like so:

```C
// Subsystem modules
#include "command_module.h"
#include "gps_module.h"
#include "mission_module.h"
#include "payload_module.h"
#include "rf_module.h"
#include "my_module.h"  // <----- Your module here
```

### Adding your module to the list of modules 
Once imported, the last thing you'd need to do is add your module to the array that loads all the modules a little further down. 

It should look something like this:
```C
// Create list of server containers
  SubsystemModule modules[] = {
      payload, gps, command, mission, rf,
  };
```

So all that'd be left to do is to add your module to the list, like so:

```C
// Create list of server containers
  SubsystemModule modules[] = {
      payload, gps, command, mission, rf, my_module,
  };
```

And that's it. 

Now when the core software is compiled, the dock will include your module and start/stop it automatically.

## Questions/comments 
Feel free to contact me about any potential questions or comments.