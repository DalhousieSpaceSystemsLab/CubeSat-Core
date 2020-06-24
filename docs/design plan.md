# Core Software Design Plan

## Mission Server
### purpose: 
automate operations on satellite once certain conditions are met.

### methods:
- method 1 
- method 2

## IPC Daemon
### purpose:
  - Run in background and wait for subsystem servers to connect.
  - Process messages in a 'dummy waitor' style for processes to communicate with each other.
  - Include minimal safeguards against disruptive usage (e.g.: spam).

### API:
  - int ipcd_init() :
    purpose: initializes the IPC daemon.
    returns: 0 if no error; -1 upon error.
  
  - int icpd_accept() :
    purpose: begin 