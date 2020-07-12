#include "ipc/client_api.h"

#define name tag

//server.c

int main()
{
  if(server_init() == -1)
  {
    fprintf(stderr, "server_init failed\n");
  }
  return 0;
}


int server_init(){
  if(ipc_connect(name) == -1) // ipc_connect()
  {
    fprintf(stderr, "ipc_connect() failed\n");
    return -1;
  }
  pthread_t await_msn;
  pthread_create(&await_msn, NULL, receiving(), NULL);
}

void receiving()
{
  char buffer[MAX_MSG_LEN];
  char fmt_buffer[MAX_MSG_LEN];
  for(;;)
  {
    ipc_recv("*", buffer, MAX_MSG_LEN);
    formatting(buffer, fmt_buffer);
    do_msn(fmt_buffer);
  }
}

void formatting(buffer, fmt_buffer)
{
  serialize(buffer, fmt_buffer)
}

void do_msn(fmt_buffer)
{
  printf("Yay i did this mission %s!", fmt_buffer);
  //add your code here you dumbasses
}
