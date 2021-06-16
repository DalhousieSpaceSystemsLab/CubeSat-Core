# LORIS Core Software documentation

## Compiling the Core Software
A guide on how to compile the repo for PC & ARM can be found in [COMPILING.md](/md_src_COMPILING.html).

## Exporting required headers/libraries to your project 
A guide on how to export the required headers and libraries to your own project can be found in [EXPORTING.md](/md_src_EXPORTING.html).

## Handy macros 
A set of useful macros were created to make using the core software nicer. You can have a look at them in [MACROS.md](/md_src_MACROS.html)

## Making modules 
A guide on how to make a module and add it to the Dock can be found in [MODULES.md](/md_src_MODULES.html).

## Inter-Process Communication (IPC) System
In order to provide a neutral means of communication between all of the
different processes and subsystems on the satellite, the IPC system was
created.

### IPC Daemon
Before doing any work with the IPC system, an instance of the IPC daemon must
be running in the background.

A guide on how to setup the daemon with some use cases can be found in [IPC_DAEMON_EXAMPLES.md](/md_src_api_libipc_examples_IPC_DAEMON_EXAMPLES.html).

### Client API
Rather than creating a custom implemention for message passing and handling
for every process, a general purpose API for the IPC system was created.

The details on how to use the API are found in client_api.h.
Examples can be found in [CLIENT_API_EXAMPLES.md](/md_src_api_libipc_examples_CLIENT_API_EXAMPLES.html).