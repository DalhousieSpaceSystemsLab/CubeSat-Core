
#include "../cc/UnixDomainServer.cc"
int main(int argc, char *argv[])
{
    UnixDomainServer power_server_("/tmp/test.sock");

    return 0;
}