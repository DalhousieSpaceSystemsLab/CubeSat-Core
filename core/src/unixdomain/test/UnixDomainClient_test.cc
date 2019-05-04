
#include "../cc/UnixDomainClient.cc"
int main(int argc, char *argv[])
{
    UnixDomainClient power_client_("/tmp/test.sock");

    return 0;
}