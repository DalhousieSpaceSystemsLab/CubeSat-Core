# IPC client API examples

This page is dedicated to documenting example use cases of the IPC client API.

## Getting started 

A handy script was created to make compiling and exporting the required headers and libraries easier.

In the root directory of the repo, run `sh export_client.sh`. A new folder titled `EXPORT_CLIENT` will be created and populated with the required headers and libraries.

## Setting up Cmake

If you happen to be using cmake, the following snippet may be useful in getting started:

```Cmake 
```


## Including the client API header

Before any API commands can be used, you must first include the `client_api.h` header. 

```C
#include "client_api.h"
```

This header is self-contained; so you don't need to worry about the other header it needs. However, attempting to compile this code on any platform besides linux will probably give you a headache. 

If you do not have this header, clone the core software repo from https://github.com/DalhousieSpaceSystemsLab/CubeSat-Core and run the `export_client.sh` shell script. This should compile all of the required libraries and put all of the required headers together.

## Connecting to the IPC daemon 

As a client, you cannot participate on the IPC network without connecting to an IPC daemon (server). See <to be made> to see how to set up the daemon.

To do this, simply run ipc_connect(). A successful return indicates that the client API has been initialized and actively connected to the daemon. 

```C
#include "client_api.h"

int main() {
  ipc_connect("pay"); // <-- Connect to the IPC daemon & register name

  // All IPC commands go here...

  ipc_disconnect(); // <-- Disconnect gracefully from the IPC daemon
}
```

When all is said and done, and participation on the IPC network is no longer desired, running the ipc_disconnect() function gracefully terminates the connection to the IPC daemon. It is possible to omit this, but the daemon won't be happy.

## Error handling 

Something to note before moving forward: almost all of the functions in the API return useful values. They usually say a thing or two about how execution went.

The expected etiquette for the usage of these API calls involves wrapping an `if()` statement around the call. Sometimes this may involve storing the return value as well.

```C
#include "client_api.h"

int main() {  
  if(ipc_connect("pay") < 0) {                        // <--- Wrap ipc_connect() with if statement
    printf("error: could not connect to the IPC\n");  // <--- Log error in case something goes wrong
    return -1;                                        // <--- Quit program with error code
  }

  ipc_disconnect();                                   // <--- Only ever returns 0, no point in checking
}
```

## Sending messages to other processes 

Sending messages via IPC is straightforward. Using the ipc_send() function, you must specify the destination, the message and the message length in bytes:

```C
int main() {
  ...
  char msg[] = "hey what's up";
  ipc_send("pwr", msg, strlen(msg));
  ...
}
```

Here is the same example, but this time we catch the return value:

```C
int main() {
  ...
  char msg[] = "hey what's up";
  if(ipc_send("pwr", msg, strlen(msg)) < 0) {
    printf("failed to send message to pwr\n");
    return -1;
  }
  ...
}
```
### Receipt confirmations

Now, the ipc_send() function has a built-in receipt confirmation handler. It expects to receive a confirmation from the destination process that the message was received within a specific timeout (see RECV_TIMEOUT in ipc_settings.h). If no receipt confirmation is received in time, the method will return an error value (-1).

So, if ipc_send() terminates in error, it is not necessarily a technical error. It may just be that the destination client is 1) not connected or 2) busy and not responding. In either case, it is considered to be a failed transmission.

**NOTE**: All receipt confirmation functionality is built-in and automated by the API; you'll never need to worry about sending or receiving confirmations.

## Receiving messages from other processes

### One-shot reception

In its simplest iteration, receiving a single message from another process requires a buffer array which will be populated by the incoming message. Using the ipc_recv() function:

```C
int main() {
  ...
  char incoming_msg[100];
  ipc_recv("*", incoming_msg, 100);
  ...
}
```

Given that we are not trying to send a message to another process, it may seem odd that there is a parameter for a process name. Even stranger is the * character.

As useful as it is to send messages to a specific destination, there may be situations where it is useful to only receive messages from a specific source. Hence the `src` field. The asterix may be used as a wildcard or 'any' entry, which means any incoming message will be accepted (regardless of source).

Here is the same example, but this time we catch the return value:

```C
int main() {
  ...
  char incoming_msg[100];
  int bytes_recvd = 0;
  if((bytes_recvd = ipc_recv("*", incoming_msg, 100)) < 0) {
    printf("failed to receive message from IPC\n");
    return -1;
  }
  ...
}
```

The ipc_recv() function is among the methods whose return value is best stored. In the example above, `bytes_recvd` keeps track of how many bytes were copied into the `incoming_msg` buffer. This tells you exactly how long the incoming message is.

**NOTE:** ipc_recv() is a blocking type method; meaning it will wait and stay stuck until a valid message is received. Not ideal for concurrent message handling. Read on to see the solution.

### Recurring, concurrent and background message reception

Suppose we wanted to up the ante on receiving messages.

Say, listening for messages from several different sources at the same time. And, using a different function to handle each incoming message according to their source.

Well we can do just that. 

#### Setting up the callback function 

The first thing to do is create the function which will be handling the incoming messages.

As long as the return type and the arguments match `void (char*, size_t, void*)`, the function can run any code and be called anything (as long as the name is unique).

Here is an example:

```C
void cb_general_msg(char* msg, size_t msg_len, void* data) {
  // general message handling goes here...
}

int main() {
  ...
}
```

The first function parameter `char* msg` is a pointer to the incoming message, `size_t msg_len` is the length of the message stored at `msg`, and `void* data` is a custom pointer that you can setup (more on this later). 

#### Creating the incoming message listener 

Using ipc_qrecv(), we can setup the message listener which will be responsible for routing incoming messages to our callback function.

Here is an example:

```C
int main() {
  ...
  ipc_qrecv("*", cb_general_msg, NULL);
  ...
}
```

This tells the API to route incoming messages from any source to the `cb_general_msg` function whenever refreshed.

We may setup a second listener for messages only coming from the power module:

```C
void cb_general_msg(char* msg, size_t msg_len, void* data){
  // general message handling goes here...
}

void cb_power_msg(char* msg, size_t msg_len, void* data) {
  // power message handling goes here...
}

int main() {
  ...
    ipc_qrecv("*", cb_general_msg, NULL);
    ipc_qrecv("pwr", cb_power_msg, NULL);
  ...
}
}
```

We now have 2 concurrent message listeners. Any messages coming in from the power module are prioritized to the `cb_power_msg` handler. Otherwise, messages go to the `cb_general_msg`.

##### The data field 

In a few situations, it is valuable to provide some data or a pointer to the callback function. Suppose we wanted the `cb_power_msg` to know the current status of our module. 

All you would need to do is provide a pointer to the value in the data field like so:

```C
int main() {
  ...
  int status = 5;
  ipc_qrecv("*", cb_general_msg, NULL);
  ipc_qrecv("pwr", cb_power_msg, &status);
  ...
}
```

Now the `cb_power_msg` has access to the `status` variable. Using it in the callback method would look something like this:

```C
...
void cb_power_msg(char* msg, size_t msg_len, void* data) {
  int status = *((int*) data);
}
...
```

**NOTE**: Be mindful of the scope of data passed along the `*data` field. It may or may not exist by the time you try to read from/to it. NULL `data` values are also accepted, so it could be something worth checking.

#### Refreshing the incoming message listeners

Once you have setup your callbacks and listeners setup, they are required to be refreshed in order for incoming messages to be actively routed to them. 

The reason for this is basically that messages coming in from the IPC sit in a 'pending' state until they are claimed by ipc_recv() or ipc_refresh(). 

Here is an example of a for loop which continuously refreshes message listeners:

```C
int main() {
  ...
  for(;;) {
    ipc_refresh();
  }
  ...
}
```

That's it. Anytime a message is received from another process, it will be routed to the respective handler function.

Here is a complete example of setting up listeners and refreshing:

```C
#include "client_api.h"

void cb_general_msg(char* msg, size_t msg_len, void* data){
  // general message handling goes here...
}

void cb_power_msg(char* msg, size_t msg_len, void* data) {
  int status = *((int*) data);
}

int main() {
  if(ipc_connect("pay") < 0) {
    printf("failed to connect to the ipc\n");
    return -1;
  }

  if(ipc_qrecv("*", cb_general_msg, NULL) < 0) {
    printf("failed to create message listener\n");
    return -1;
  }
  
  int status = 5;
  if(ipc_qrecv("pwr", cb_power_msg, &status) < 0) {
    printf("failed to create message listener\n");
    return -1;
  }

  for(;;) {
    ipc_refresh();
  }

  return 0;
}
```

#### Refreshing specific message listeners

This may not happen often, but if ever you wish to only actively route messages from a specific source (ignoring any other messages or handlers), you may do so using the ipc_refresh_src() method:

```C
int main() {
  ...
  for(;;) {
    ipc_refresh_src("pwr");
  }
  ...
}
```

The example above would only refresh the listener for the `pwr` listener we created earlier. This would be the case regardless of the number or presence of other listeners. 

**NOTE**: Calling `ipc_refresh()` is equivalent to `ipc_refresh_src("*")`. 

## Issues, comments or questions 

In any case of the above, do not hesitate in sending me a message on slack @Alex or via email loris@alexamellal.com .
