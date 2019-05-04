#include "../header/UnixDomainClient.h"


/*
	A simple client in the internet domain using TCP
*/
//REF: http://www.linuxhowtos.org/C_C++/socket.htm
UnixDomainClient::UnixDomainClient(char sun_path[]){
    OpenSocket();
    SetHostServer();
    ConnectToServer(sun_path);
    WriteToServer();
    ReadFromServer();
}

void UnixDomainClient::ConnectToServer(char sun_path[]) {
    bzero((char *) &serv_addr_, sizeof(serv_addr_));
    serv_addr_.sun_family = AF_UNIX;


    strcpy(serv_addr_.sun_path, sun_path);

    servlen = strlen(serv_addr_.sun_path) +
              sizeof(serv_addr_.sun_family);

    std::cout << "Attempting connection to socket " << this->sockfd_ << std::endl;
    if (connect(sockfd_, (struct sockaddr *) &serv_addr_, sizeof(serv_addr_)) < 0)
        error("ERROR connecting");
}

void UnixDomainClient::SetHostServer() {
    server_ = gethostbyname("localhost");
    if (server_ == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
}

void UnixDomainClient::OpenSocket() {
    sockfd_ = socket(AF_UNIX, SOCK_STREAM, 0);

    if (sockfd_ < 0)
        error("ERROR opening socket");
}

void UnixDomainClient::WriteToServer() {
    printf("Please enter the message: ");

    bzero(this->buffer_, 256);

    fgets(this->buffer_, 255, stdin);

    n_ = write(this->sockfd_, this->buffer_, strlen(this->buffer_));

    if (n_ < 0)
        error("ERROR writing to socket");
}

void UnixDomainClient::ReadFromServer() {
    bzero(buffer_, 256);

    n_ = read(sockfd_, buffer_, 255);

    if (n_ < 0)
        error("ERROR reading from socket");
    printf("%s", buffer_);
}

void UnixDomainClient::error(const char *msg) {
    perror(msg);
    exit(1);
}
