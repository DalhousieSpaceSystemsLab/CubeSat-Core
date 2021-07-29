# IPC client API examples
This page is dedicated to documenting example use cases of the IPC client API.

## Getting started 
A handy script was created to make compiling and exporting the required headers and libraries easier.

A guide on how to use it and get set up in your project can be found in [EXPORTING.md](/md_src_EXPORTING.html)

## Assumptions 
Brief overvivew at some of the assumptions made in this guide.

### 1. You're making or planning to make a module
Creating modules is an important part of the core software. As almost everything on the OBC is intended to run as a module on the dock.

For this reason, this guide assumes you will (at some point) be trying to create a module, so the examples are given within that context. However, implementing these methods in your own project will be very similar. 

The main difference would be that the macros would not be available to you unless you link the `libLIB_IPC.a` library and include the `subsysmod.h` and `modutil.h` headers. Totally doable, maybe send me a message to get set up.

Otherwise, you can ignore the macros and make your own equivalents. Most of them are pretty simple and can be replaced with `if` statements or normal methods like `printf()`.

### 2. You've learned about some of the macros 
There's a handy guide on the core software macros in [MACROS.md](/md_src_MACROS.html). They will be used in the following examples to make things cleaner, so it may help you understand this guide to have a quick look at.

Even if you are not planning on using them, the guide will help you figure out what the simple equivalents would be.

## Including the necessary headers 
For any of the IPC or core software methods to work, the necessary headers must be present. 

If you are working on a module and intend to use the full package, you may include subsysmod.h by doing the following:
```C
#include "subsysmod.h"
```
Simple as that.

Otherwise, if you are implementing the IPC client in a separate project and do not want to use the additional macros, the client_api.h should do. 
It may be imported like so:
```C
#include "client_api.h"
```

Your exact configuration may vary, so feel free to adjust the header path accordingly.

## Connecting to the IPC daemon 
As a client, you cannot participate on the IPC network without connecting to an IPC daemon (server). See [IPC_DAEMON_EXAMPLES.md](/md_src_api_libipc_examples_IPC_DAEMON_EXAMPLES.html) to see how to set up the daemon.

To do this, simply run ipc_connect(). A successful return indicates that the client API has been initialized and actively connected to the daemon. 

```C
...

ipc_connect("pay"); // <-- Connect to the IPC daemon & register name

// All IPC commands go here...

ipc_disconnect(); // <-- Disconnect gracefully from the IPC daemon

...
```

When all is said and done, and participation on the IPC network is no longer desired, running the ipc_disconnect() function gracefully terminates the connection to the IPC daemon. It is possible to omit this, but the daemon won't be happy.

## Error handling 

Something to note before moving forward: almost all of the functions in the API return useful values. They usually say a thing or two about how execution went.

The expected etiquette for the usage of these API calls involves wrapping it with `OK()`, one of the useful macros mentioned earlier.

```C
...

OK(ipc_connect("pay"));   // <-- Runs ipc_connect() and checks return value

...
```
Or equivalently,
```C
...

if(ipc_connect("pay") < 0) {                        // <-- Wrap ipc_connect() with if statement
  printf("error: could not connect to the IPC\n");  // <-- Log error in case something goes wrong
  return -1;                                        // <-- Quit program with error code
}

...
```
From now on, we will be sticking to the `OK()` macro for error checking.

## Sending messages to other processes 

Sending messages via IPC is straightforward. Using the ipc_send() function, you must specify the destination, the message and the message length in bytes:

```C
...

char msg[] = "hey what's up";
ipc_send("pwr", msg, strlen(msg));

...
```

Here is the same example, but this time we catch the return value:

```C
...

char msg[] = "hey what's up";
OK(ipc_send("pwr", msg, strlen(msg)));

...
}
```
### Receipt confirmations

Now, the ipc_send() function has a built-in receipt confirmation handler. It expects to receive a confirmation from the destination process that the message was received within a specific timeout (see RECV_TIMEOUT in ipc_settings.h). If no receipt confirmation is received in time, the method will return an error value (-1).

So, if ipc_send() terminates in error, it is not necessarily a technical error. It may just be that the destination client is 1) not connected or 2) busy and not responding. In either case, it is considered to be a failed transmission.

The timeout for receipt confirmations in defined in `ipc_settings.h` under `RECV_TIMEOUT`.

**NOTE**: All receipt confirmation functionality is built-in and automated by the API; you'll never need to worry about sending or receiving confirmations.

## Receiving messages from other processes

### One-shot reception

In its simplest iteration, receiving a single message from another process requires a buffer array which will be populated by the incoming message. Using the ipc_recv() function:

```C
...

char incoming_msg[100];
ipc_recv("*", incoming_msg, 100, NO_TIMEOUT);

...
```

Given that we are not trying to send a message to another process, it may seem odd that there is a parameter for a process name. Even stranger is the * character.

As useful as it is to send messages to a specific destination, there may be situations where it is useful to only receive messages from a specific source. Hence the `src` field. The asterix may be used as a wildcard or 'any' entry, which means any incoming message will be accepted (regardless of source).

Here is the same example, but this time we catch the return value:

```C
...

  char incoming_msg[100];
  int bytes_recvd = 0;
  OK(bytes_recvd = ipc_recv("*", incoming_msg, 100, NO_TIMEOUT));

...
```

The ipc_recv() function is among the methods whose return value is best stored. In the example above, `bytes_recvd` keeps track of how many bytes were copied into the `incoming_msg` buffer. This tells you exactly how long the incoming message is.

**NOTE:** ipc_recv() is a blocking type method; meaning it will wait and stay stuck until a valid message is received OR the timeout is exceeded (in seconds). Not ideal for concurrent message handling. Read on to see the solution.

### Recurring, concurrent and background message reception

Suppose we wanted to up the ante on receiving messages.

Say, listening for messages from several different sources at the same time. And, using a different function to handle each incoming message according to their source.

Well we can do just that. 

#### Setting up the callback function 

The first thing to do is create the function which will be handling the incoming messages.

As long as the return type and the arguments match `int (char*, size_t, void*)`, the function can run any code and be called anything (as long as the name is unique).

Luckily there's a macro for that: `CALLBACK(name)`.

Here is an example:

```C
...

CALLBACK(general) {
  // do stuff with incoming message
}

...

```

Although invisible when using the macro, there are 3 function parameters that are automatically passed to the callback function. They are:
- `char* src` ==> pointer to name of the message source,
- `char* msg` ==> pointer to the incoming message, 
- `size_t msg_len` ==> length of the message stored at `msg`, 
- `void* data` ==> custom pointer that you can setup (more on this later). 

An example use case of these parameters:

```C
...

CALLBACK(general) {
  modprintf("Incoming message from %s: \n", src);   // <-- char src[3]
  modprintf("Message content: %s\n", msg);          // <-- char* msg
  modprintf("Message length: %d\n", msg_len);       // <-- size_t msg_len
  modprintf("Data pointer: %p\n", data);            // <-- void* data (may be NULL)
}

...
```

#### Creating the incoming message listener 

Using `ipc_create_listener()`, we can setup the message listener which will be responsible for routing incoming messages to our callback function.

Here is an example:

```C
...

CALLBACK(general) {
  // do stuff with incoming message
}

START_MODULE(my_module) {
  ...

  OK(ipc_create_listener("*", general, NULL));

  ...
}

...


This tells the API to route incoming messages from any source (`"*"`) to the `my_callback` callback whenever refreshed.

We may setup a second listener which only listens for messages from a single source; say from the power module:

```C
...

CALLBACK(general) {
  // do stuff with general incoming messages
}

CALLBACK(power) {
  // do stuff with incoming messages from power
}

START_MODULE(my_module) {
  ...

  OK(ipc_create_listener("*", general, NULL));
  OK(ipc_create_listener("pwr", power, NULL));
  
  ...
}

...
```

We now have 2 concurrent message listeners. Any messages coming in from the power module are prioritized to the `power` callback. Otherwise, messages go to `general`.

##### The data field 

In a few situations, it is valuable to provide some data or a pointer to the callback function. Suppose we wanted the `power` callback to know the current status of our module. 

All you would need to do is provide a pointer to the value in the data field like so:

```C
...

START_MODULE(my_module) {
  ...

  int status = 5;

  OK(ipc_create_listener("*", general, NULL));    // <-- We keep the data pointer NULL; nothing special.
  OK(ipc_create_listener("pwr", power, &status)); // <-- We pass a pointer to the status
  
  ...
}

...
```

Now the `power` callback has access to the `status` variable using the `data` pointer. Using it in the callback method would look something like this:

```C
...

CALLBACK(power) {
  ...

  // Store status locally 
  int status = *((int*) data); //<-- Weird pointer trick to get an int out of a void*.

  // Do something with status...

  ...
}

...
```

**NOTE**: Be mindful of the scope of data passed along the `*data` field. It may or may not exist by the time you try to read from/to it. NULL `data` values are also accepted, so it is something worth checking.

#### Refreshing the incoming message listeners

Once you have your callbacks and listeners setup, they are required to be refreshed for incoming messages to be actively routed to them. 

Here is an example of a `for` loop which continuously refreshes message listeners:

```C
...

START_MODULE(my_module) {
  ...

  for(;;) {
    OK(ipc_refresh());
  }

  ...
}

...
```

That's it. Anytime a message is received from another process, it will be routed to the respective handler function.

Here is a complete example of setting up listeners and refreshing:

```C
// my_module.c

#include "subsysmod.h"

CALLBACK(general) {
  // do stuff with general incoming messages
}

CALLBACK(power) {
  // Store status locally 
  int status = *((int*) data); //<-- Weird pointer trick to get an int out of a void*.
}

START_MODULE(my_module) {
  OK(ipc_connect("pay"));                         // <-- Connect to the IPC

  int status = 5;                                 // <-- Create status variable

  OK(ipc_create_listener("*", general, NULL));    // <-- We keep the data pointer NULL; nothing special.
  OK(ipc_create_listener("pwr", power, &status)); // <-- We pass a pointer to the status

  for(;;) {
    OK(ipc_refresh());                                // <-- Keep refreshing for incoming messages
  }

  return 0;
}
```

## Issues, comments or questions 

In any case of the above, do not hesitate in sending me a message on slack @Alex or via email loris@alexamellal.com .
