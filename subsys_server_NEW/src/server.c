#include <pthread.h>

#include "client_api.h"

/** @note Before refactor, this was >#define name tag<
 * but I couldn't find a symbol for tag so I quoted as a string litteral
 * - Carl
 * */
#define name "tag"

static int   server_init(void);
static void *receiving(void *args);

static void  formatting(buffer,
                       fmt_buffer); /** @note I'm very curious if this will be
                                        ABI compatible with the C++ code... */
static void do_msn(fmt_buffer);

// server.c

int main()
{
    if (server_init() == -1)
    {
        fprintf(stderr, "server_init failed\n");
    }
    return 0;
}


static int server_init(void)
{
    if (ipc_connect(name) == -1) // ipc_connect()
    {
        fprintf(stderr, "ipc_connect() failed\n");
        return -1;
    }
    pthread_t await_msn;
    pthread_create(&await_msn, NULL, receiving, NULL);
}

static void *receiving(void *args)
{
    char buffer[MAX_MSG_LEN];
    char fmt_buffer[MAX_MSG_LEN];
    for (;;)
    {
        ipc_recv("*", buffer, MAX_MSG_LEN);
        formatting(buffer, fmt_buffer);
        do_msn(fmt_buffer);
    }
}


/* Reminsicient of K&R C from 1971 haha */
static void formatting(buffer, fmt_buffer)
{
    serialize(buffer, fmt_buffer);
}

static void do_msn(fmt_buffer)
{
    printf("Yay i did this mission %s!", fmt_buffer);
    // add your code here
}
