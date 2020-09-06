# Subsystem Servers

By name, description and function.

## List

- Comms server
- Command server
- Mission server
- Payload server
- ACS server
- Power server
- Watchdog server
- Filesystem server

## Descriptions

### Comms server

Handles the satellite's comms subsystem. Translates incoming raw transmitter data into useful commands, which are forwarded to the Command server for interpretation.

Also translates useful data into raw transmitter data to be sent back down to the groundstation.

### Command server

Interprets commands received from the ground station (Comms server) and translates them into standardized IPC compatible commands.

Once interpreted, the command is issued to the apropriate server to begin execution.

### Mission server

If a command interpreted by the Command server is found to be designated a future time or coordinate to be executed, the command is forwarded to the mission server.

Once the designated conditions are met, the command is issued to the apropriate server to begin execution.
