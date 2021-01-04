#include <pthread.h>

#include "client_api.h"

#include "serialize.hpp"

/** @note Before refactor, this was >#define name tag<
 * but I couldn't find a symbol for tag so I quoted as a string litteral
 * - Carl
 * */
#define name "tag"

static int   server_init(void);
static void *receiving(void *);
static void  formatting(char *buffer, char *fmt_buffer);
static void  do_msn(char *fmt_buffer);


// server.c

int main(void)
{
    if (server_init() == -1)
    {
        fprintf(stderr, "server_init failed\n");
    }
    return 0;
}


static int server_init(void)
{
    /* sneak past certain compiler warnings */
    const char *const_name_ptr = (const char *)name;
    if (ipc_connect(const_name_ptr) == -1) // ipc_connect()
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

static void formatting(char *buffer, char *fmt_buffer)
{   
    #warning NO IDEA WHAT THIS IS SUPPORTED TO DO BECAUSE WE ALREADY HAVE THE PARAMS IN char* format (not std::map)
    //dss::ipc::util::serialize(buffer, fmt_buffer);
}

static void do_msn(char *fmt_buffer)
{
    // printf("Yay i did this mission %s!", fmt_buffer);
    // add your code here
}
