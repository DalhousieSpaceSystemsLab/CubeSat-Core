# IPC Daemon Examples

This page is dedicated to documenting example use cases of the IPC daemon.

## Getting started 

Before doing any work with the IPC system, an instance of the IPC daemon must be running in the background. This allows clients to use the IPC client API to send and receive messages via the IPC network.

## Running the daemon

Once everything compiles successfully, there should be a binary in the `bin/x86/` directory called `IPC_DAEMON.out`.
If you haven't compiled the repo yet, have a look at [COMPILING.md](/md_src_COMPILING.html).

Typically, I like to run the binaries from the root directory of the repo, but you may do so from any directory. Keep in mind that from whichever directory you run the IPC daemon, you must also call the client or dock binary.

### Quick example

Here's how I like to run the daemon:

```sh
./bin/x86/IPC_DAEMON.out
```

## Using the IPC daemon in your project 

Since the IPC daemon is a standalone binary (and the code isn't meant to be portable), you can go right ahead and copy the entire `IPC_DAEMON.out` executable to wherever you want. Once copied over, it should work right out of the box.

# Resources 

If ever you have any questions or feedback, don't hesitate contacting me @Alex on slack or via email at loris@alexamellal.com.