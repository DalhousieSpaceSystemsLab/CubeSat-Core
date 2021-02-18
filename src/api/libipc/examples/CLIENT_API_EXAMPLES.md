# IPC client API examples

This page is dedicated to documenting example use cases of the IPC client API.

## Getting started 

Before any API commands can be used, you must first include the `client_api.h` header. 

```C
#include "client_api.h
```

This header is self-contained; so you don't need to worry about the other header it needs. However, attempting to compile this code on any platform besides linux will probably give you a headache. 

If you do not have this header, clone the core software repo from https://github.com/DalhousieSpaceSystemsLab/CubeSat-Core and run the `export_client.sh` shell script. This should compile all of the required libraries and put all of the required headers together.

## Connecting to the IPC daemon 

As a client, you cannot participate on the IPC network without connecting to a daemon (server). See <to be made> to see how to set up the daemon.

```C
#include "client_api.h"

int main() {
  ipc_connect("pay");

  // API commands here

  ipc_disconnect();
}
```