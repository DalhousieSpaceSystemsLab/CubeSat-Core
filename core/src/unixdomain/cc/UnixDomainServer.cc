#include "../header/UnixDomainServer.h"

/*
	A simple server in the unix domain using unix domain socket
*/
//REF: http://www.linuxhowtos.org/C_C++/socket.htm

void UnixDomainServer::BindSocketToSockAddress(int socket_file_descriptor, struct sockaddr_un server_address) {
    //Bind the socket to an IP address and port
    if (bind(socket_file_descriptor, (struct sockaddr *) (&server_address), sizeof(server_address)) < 0) {
        cout << "server_address_ size: " << sizeof(server_address) << endl;
        cout << "Error code: " << errno << endl;
        error("ERROR on binding");
    }
}

UnixDomainServer::UnixDomainServer(char sun_path[]) {
    cout << "Constructor: socket path: " << sun_path << endl;
    //TODO(andrewway) Clean up constructor. Break up into functions
    this->path_ = sun_path;

    //Initialize the server_address_ struct
    ResetBuffer();

    server_address_.sun_family = AF_UNIX;
    strcpy(server_address_.sun_path, sun_path);

    //AF_UNIX is to be used for communication between sockets on the same host system
    socket_file_descriptor_ = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_file_descriptor_ < 0) {
        error("ERROR opening socket");
    }

    int enable = 1;
    if (setsockopt(socket_file_descriptor_, SOL_SOCKET, SO_REUSEADDR,
                   &enable, sizeof(int)) < 0) {
        error("setsockopt(SO_REUSEADDR) failed");
    }


    ToString();

    BindSocketToSockAddress(socket_file_descriptor_, server_address_);

    cout << "Listening on socket " << sun_path << "..." << endl;
    //Indicate that the socket is for listening
    listen(socket_file_descriptor_, 5);

    WaitForConnection();

}

//TODO Add a virtual function that allows the server to perform some operation in between waiting
//TODO Find a way to continue looping IF there are no waiting clients. RIght now it just pauses.
//TODO Checkout "fcntl". May potentially allow non-blocking mode
void UnixDomainServer::WaitForConnection() {
    while (true) {
        cout << "Waiting for connection..." << endl;
        //Wait for connection
        client_address_size = sizeof(client_address_);
        new_socket_file_descriptor_ = accept(socket_file_descriptor_,
                                             (struct sockaddr *) &client_address_, &client_address_size);
        //TODO Avoid blocking? We need to keep looping sometimes when there is no client waiting.
        //accept() blocks until connection is made
        if (new_socket_file_descriptor_ < 0)
            error("ERROR on accept");

        //Code only proceeds beyond this point if connection was made
        cout << "Handling new request" << endl;
        ReceiveRequest(buffer_, new_socket_file_descriptor_);
    }
}

void UnixDomainServer::ResetBuffer() {
    //Set buffer to zeros
    bzero((char *) &server_address_, sizeof(server_address_));
}

void UnixDomainServer::ToString() {
    //TODO(andrewway) Change to return string
    cout << "socket_file_descriptor_: " << socket_file_descriptor_ << endl;
    cout << "AF_UNIX: " << server_address_.sun_family << endl;
    cout << "server_address_.sin.s: " << server_address_.sun_path << endl;
    cout << "&server_address_: " << &server_address_ << endl;
    cout << "server_address_ size: " << sizeof(server_address_) << endl;
}


int UnixDomainServer::ReceiveRequest(char *buffer, int new_socket_file_descriptor) {
    bzero(buffer, 256);

    int socket_io_status = read(new_socket_file_descriptor, buffer, 255);
    if (socket_io_status < 0)
        error("ERROR reading from socket");
    printf("Here is the message: %s", buffer);

    int request_status = HandleRequest(buffer, new_socket_file_descriptor);

    WriteToClient("Message received! Disconnecting you now.", new_socket_file_descriptor);

    return 0;
}

void UnixDomainServer::WriteToClient(const char *msg, int new_socket_file_descriptor) {
    //TODO Make sure buffer is large enough to hold the entire message!
    int socket_io_status = write(new_socket_file_descriptor, msg, 18);
    if (socket_io_status < 0)
        error("ERROR writing to socket");
}

void UnixDomainServer::error(const char *msg) {
    perror(msg);
    exit(1);
}/**/
