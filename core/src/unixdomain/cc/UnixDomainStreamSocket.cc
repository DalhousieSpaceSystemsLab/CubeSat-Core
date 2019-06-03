//
// Created by Spencer Axford on 2019-05-06.
//

#include "UnixDomainStreamSocket.h"

//Sets up connection to socket file
int UnixDomainStreamSocket::InitializeSocket(char sun_path[]) {
    //Initialize the socket_address_ struct
    ClearAddress();

    this->path_ = sun_path;
    socket_address_.sun_family = AF_UNIX;
    strcpy(socket_address_.sun_path, sun_path);

    cout << "Initialize socket path: " << socket_address_.sun_path << endl;

    //AF_UNIX is to be used for communication between sockets on the same host system
    socket_file_descriptor_ = socket(socket_address_.sun_family, SOCK_STREAM, 0);
    if (socket_file_descriptor_ < 0) {
        error("ERROR opening socket");
        return 1;
    }

    int enable = 1;
    if (setsockopt(socket_file_descriptor_, SOL_SOCKET, SO_REUSEADDR,
                   &enable, sizeof(int)) < 0) {
        error("setsockopt(SO_REUSEADDR) failed");
        return 1;
    }
    cout << "Socket Initialized" << endl;
    return 0;
}

//Write msg to the socket specified by the sfd
int UnixDomainStreamSocket::WriteToSocket(const char *msg, int new_socket_file_descriptor) {

    std::cout << "THE MESSAGE SHOULD BE" << msg << std::endl;
    float *q = (float*)msg;
	std::cout << q <<  " || " << *msg << std::endl;
    unsigned int sender_ = (unsigned int) *q;   q++;
	std::cout << sender_ << std::endl;
    unsigned int reciever_ = (unsigned int) *q; q++;
	std::cout << reciever_ << std::endl;
    long time_sent_ = (long) *q;                q++;
	std::cout << time_sent_ << std::endl;

    char mess[255] = {static_cast<char>(sender_), static_cast<char>(reciever_), static_cast<char>(time_sent_), 'F'};

    strcpy(this->buffer_, msg);
    n_ = write(new_socket_file_descriptor, this->buffer_, strlen(this->buffer_));

    if (n_ < 0) {
        error("ERROR writing to socket");
        return 1;
    }
    ResetBuffer();

    return 0;
}

//Read from the socket specified by the sfd
int UnixDomainStreamSocket::ReadFromSocket(int new_socket_file_descriptor) {
    ResetBuffer();

    n_ = read(new_socket_file_descriptor, buffer_, 255);

    if (n_ < 0) {
        error("ERROR reading from socket");
        return 1;
    }
    printf("Here is the message: %s \n", buffer_);
    return HandleMessage(buffer_);
}

//Prints error messages
void UnixDomainStreamSocket::error(const char *msg) {
    perror(msg);
}

//Sets buffer_ to an array of 0's
void UnixDomainStreamSocket::ResetBuffer() {
    bzero(buffer_, 256);
}

//Clears all fields in the socket_address_ struct
void UnixDomainStreamSocket::ClearAddress() {
    bzero((char *) &socket_address_, sizeof(socket_address_));
}