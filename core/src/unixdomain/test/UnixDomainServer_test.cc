
#include "../example/UDServerExample.cc"
int main(int argc, char *argv[])
{
    UDServerExample power_server_("/tmp/woahtest.sock");
    power_server_.WaitForConnection();

    return 0;
}