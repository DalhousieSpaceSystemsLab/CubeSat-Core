// trying to do some functional C style programming...

#include "ipc/client_t.h"

#include <sys/socket.h>
#include <sys/un.h>

const int get_socket();

int main()
{
  
  // done
  return 0;
}

const int get_socket()
{
  static int static_sock = -1;

  if(static_sock == -1) 
  {
    const int sock = socket(AF_UNIX, SOCK_STREAM, 0);
    static_sock = sock;

    return sock;
  }

  return static_sock;
}