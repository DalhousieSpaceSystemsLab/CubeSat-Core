# Useful macros 
A set of useful macros were created to make using the core software nicer. 

## Module macros 
These macros can be found in the subsysmod.h header as part of libipc.

### Creating modules 
Creating modules is an important part of the core software. As almost everything on the OBC is intended to run as a module on the dock, the following modules were made to simplify this process:

- `NEW_DOCK_MODULE(name)` ==> Declares the subsystem module in the module header.
- `EXPORT_MODULE(name)` ==> Initializes the subsystem module at the end of the module source file.
- `START_MODULE(name) { }` ==> Start method for the module; contains setup and primary runtime calls.
- `STOP_MODULE(name) { }` ==> Stop method for the module; run once when the dock is stopping the module.

An example module could be created like:
```C
// my_module.h 

#ifndef __MY_MODULE_H__
#define __MY_MODULE_H__

#include "subsysmod.h"

NEW_DOCK_MODULE(my_module);

#endif
```

```C
// my_module.c

#include "my_module.h"

START_MODULE(my_module) {
  // do stuff 
}

STOP_MODULE(my_module) {
  // close stuff
}

EXPORT_MODULE(my_module);
```
That's it.

Only thing left to do would be to add the module to the list of modules in `src/dock/main.c`.

### Creating background listeners 
The following macros were created to make creating background listeners for the IPC easier:
- `CALLBACK(name) { }` ==> Creates a callback method for an IPC background listener called `name`.
- `STOP_CALLBACK` ==> Returns the apropriate value to end a callback; basically equivalent to `return 0` but this may change.

An example of this could look like:
```C
// my_module.c

#include "my_module.h"

CALLBACK(general) {
  // do stuff with msg 
  STOP_CALLBACK;
}

START_MODULE(my_module) {
  // ...
  ipc_create_background_listener("*", general, NULL);
  // ...
}

STOP_MODULE(my_module) {
  // close stuff
}

EXPORT_MODULE(my_module);
```

## Module Utilities 
These macros can be found in the modutil.h header as part of libipc.

### Custom printf() for modules
Given that all modules run on the same executable, it becomes difficult to distinguish which module is writing which output to stdout. 

To address this, several macros which wrap `printf()` were created. They can all be used exactly the same way `printf()` is used, but automatically include information like which module is writing the output, was it an error or an informative message, etc.

#### Informative printing 
To print general information or prompts, use `modprintf()`:
```C
int x = 5;
modprintf("This is a message with formatting, like x = %d\n", x);
```
As you can see, inline formatting with arguments like integers, strings and the like is supported, just like `printf()`. 

The resulting output to the terminal would look something like this:
```
[i](start_module_xxx) This is a message with formatting, like x = 5
```

#### Error printing 
To print errors, use `moderr()`:
```C
int errno = -1;
moderr("COULD NOT COMPLETE X, error number %d\n", errno);
```
Like `modprintf()`, `moderr()` also supports inline formatting and arguments.

The resulting output would look something like this:
```
[!](start_module_xxx) COULD NOT COMPLETE X, error number -1
```

### Easy exception catcher 
All functions in the core software are (currently) meant to return 0 or a positive value on success, and -1 on error. 

The `OK()` macro was created to conveniently check the return value of some function call, and if it is less than 0, to print an error message and return -1.

An example of its usage would be like:
```C
OK(ipc_connect("mod"));

OK(ipc_send("pay", "waddup", 6));

OK(ipc_disconnect());
```

### Function timeout handling 
Timeouts can be useful to have built-in to functions, especially when everything needs to be automated.

A series of `TIMEOUT` macros were created, and they are as follows:
- `TIMEOUT_START()` ==> Initializes timeout variables & sets start time
- `TIMEOUT_UPDATE()` ==> Updates the time elapsed; usually appears in loops.
- `TIMEOUT_IF(max_time, action)` ==> Checks if timer has exceeded `max_time`; if it has, perform `action`.
- `RETURN_IF_TIMEOUT()` ==> Returns `NO_TIMEOUT` or `TIMEOUT_OCCURED` ints based on whether a timeout occured.
- `IF_TIMEOUT(func, action)` ==> Checks if timeout containing function `func` returned `NO_TIMEOUT` or `TIMEOUT_OCCURED`. If timeout did occur, `action` is performed.
- `NO_TIMEOUT` ==> Value is 0.
- `TIMEOUT_EXCEEDED` ==> Value is -111.

**NOTE** `action` can include code, like `break`, `continue` or `return`.

An example use case could be as follows:
```C
int foo(int timeout) {
  TIMEOUT_START();
  for(;;) {
    do_work();

    TIMEOUT_UPDATE();
    TIMEOUT_IF(timeout, break);
  }

  RETURN_IF_TIMEOUT();
}

int main() {
  IF_TIMEOUT(foo(3), printf("timed out\n"));
  return 0;
}
```