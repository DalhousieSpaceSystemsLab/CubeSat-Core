# IPC Daemon Examples

This page is dedicated to documenting example use cases of the IPC daemon.

## Getting started 

Before doing any work with the IPC system, an instance of the IPC daemon must be running in the background. This allows clients to use the IPC client API to send and receive messages via the IPC network.

### Cloning the core software repo

Head over https://github.com/DalhousieSpaceSystemsLab/CubeSat-Core to find the core software repo. 

### Compiling the core software

Once cloned, `cd CubeSat-Core`. You may choose to use the master branch (default) or switch to the develop branch for the latest developments. This can easily be done by running `git switch develop`.

## Running the daemon

Once everything compiles successfully, there should be a binary in the `build` directory called `IPC_DAEMON.out`. 

Typically, I like to run the binaries from the root directory of the repo, but you may do so from any directory. Keep in mind that from whichever directory you run the IPC daemon, you must also call the client binary.

### Why must I run the IPC daemon and clients FROM the same directory?

Try running the IPC daemon once and quitting right away. If you take a look at the directory where you entered the call to execute `IPC_DAEMON.out`, you'll find an automatically generated `socket.socket` file. This is the named pipe used for the initial connection socket of the IPC. Without it, there would be no way for a client to connect to the IPC.

By default, calling the `IPC_CLIENT.out` binary from any directory causes it to look for a `socket.socket` in the directory in which it was called. So, that is why all of the binaries, daemon and clients alike, must be called **from** the same directory (but dont need to be in the same directory).

### Just show me an example that will work

Here is what I usually like to do:

```sh
# To start the IPC Daemon
cd CubeSat-Core
./build/IPC_DAEMON.out

# To start the IPC Client 
cd CubeSat-Core
./build/IPC_CLIENT.out <name> <mode>
```

Since in both cases we are running the binaries from `CubeSat-Core` directory, the `socket.socket` file used by both the daemon and client correspond to each other.

## Using the IPC daemon in my project 

Since the IPC daemon is a standalone binary (and the code isn't meant to be portable), you can go right ahead and copy the entire `IPC_DAEMON.out` executable to wherever you want. Once copied over, it should work right out of the box.

# Resources 

If ever you have any questions or feedback, don't hesitate contacting me @Alex on slack or via email at loris@alexamellal.com.