/*
* tests.h
*
*   purpose: Unit testing using cmocka library for core software.
*   author: alex amellal
*
*/

#ifndef CUBESAT_CORE_INCLUDE_IPC_TESTS_TEST_CLIENT_API_H
#define CUBESAT_CORE_INCLUDE_IPC_TESTS_TEST_CLIENT_API_H

// IPC Client API 
#include "ipc/settings.h"
#include "ipc/client_api.h"

// IPC daemon
#include "ipc/ipcd.h"

// Private keyword 
#include "util/private.h"

// Standard C libraries 
#include <pthread.h>

// CMocka libraries
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#define DEBUG 1

// Test wrappers 
int __wrap_socket(int domain, int type, int protocol);
int __wrap_connect(int sock, struct sockaddr * address, socklen_t address_len);
int __wrap_write(int sock, void * buf, size_t buflen);
int __wrap_read(int sock, void * buf, size_t buflen);
int __wrap_close(int sockfd);
int __wrap_unlink(const char path[]);
int __wrap_bind(int sockfd, struct sockaddr * address, socklen_t address_len);
int __wrap_listen(int sockfd, int n);


// Test IPC client API
void test_client_api_connect();
void test_client_api_send();
void test_client_api_recv();
void test_client_api_refresh();
void test_client_api_disconnect();

// Test IPC daemon 
void test_ipcd_init();
void test_ipcd_close();
void test_ipcd_start_accepting();

#endif // CUBESAT_CORE_INCLUDE_IPC_TESTS_TEST_CLIENT_API_H