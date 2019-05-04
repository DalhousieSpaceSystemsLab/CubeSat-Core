
#include "../cc/UnixDomainServer.cc"
int main(int argc, char *argv[])
{
    UnixDomainServer power_server_("/tmp/mytest.sock");


    return 0;
}