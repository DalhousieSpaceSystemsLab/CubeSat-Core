#include "UnixDomainStreamSocketServer.h"

/*
	A simple server in the unix domain using unix domain socket
*/
//REF: http://www.linuxhowtos.org/C_C++/socket.htm

UnixDomainStreamSocketServer::UnixDomainStreamSocketServer(string sock_path) {

    int n = sock_path.length();

    // declaring character array
    char socket_path[n + 1];

    // copying the contents of the
    // string to char array
    strcpy(socket_path, sock_path.c_str());

    cout << "Constructor: socket path: " << socket_path << endl;
    if (InitializeSocket(socket_path) != 0) {
        error("Error Initializing Socket");
    }

    ToString();
    BindSocketToAddress(socket_file_descriptor_, socket_address_);

}

//TODO change return type to int to indicate success/failure and abort socket setup (if failed)
void UnixDomainStreamSocketServer::BindSocketToAddress(int socket_file_descriptor, struct sockaddr_un socket_address) {
    //Bind the socket to an IP address and port
    cout << "unlinking" << endl;
    unlink(socket_address.sun_path);
    cout << "binding" << endl;
    cout << "socket fd: "<< socket_file_descriptor << endl;
    if (bind(socket_file_descriptor, (struct sockaddr *) (&socket_address), sizeof(socket_address)) < 0) {
        cout << "socket_address_ size: " << sizeof(socket_address) << endl;
        cout << "Error code: " << errno << endl;
        error("ERROR on binding");
    }
}

//TODO Add a virtual function that allows the server to perform some operation in between waiting
//TODO Find a way to continue looping IF there are no waiting clients. RIght now it just pauses.
//TODO Checkout "fcntl". May potentially allow non-blocking mode
void UnixDomainStreamSocketServer::WaitForConnection() {
    cout << "Listening on socket " << socket_address_.sun_path << "..." << endl;
    //Indicate that the socket is for listening
    listen(socket_file_descriptor_, 5);

   // while (true) {
        cout << "Waiting for connection..." << endl;
        //Wait for connection
        client_address_size = sizeof(client_address_);
        new_socket_file_descriptor_ = accept(socket_file_descriptor_,
                                             (struct sockaddr *) &client_address_, &client_address_size);
        //TODO Avoid blocking? We need to keep looping sometimes when there is no client waiting.
        //accept() blocks until connection is made
        if (new_socket_file_descriptor_ < 0)
            error("ERROR on accept");
        else{
        	//Code only proceeds beyond this point if connection was made. Is this true?
        	cout << "Handling new request" << endl;
            ReadFromSocket(new_socket_file_descriptor_, 255);
        	// this->HandleConnection(new_socket_file_descriptor_);
        }
   // }
}

// Clean way to print server info
void UnixDomainStreamSocketServer::ToString() {
    //TODO(andrewway) Change to return string
    cout << "socket_file_descriptor_: " << socket_file_descriptor_ << endl;
    cout << "AF_UNIX: " << socket_address_.sun_family << endl;
    cout << "socket_address_.sun_path: " << socket_address_.sun_path << endl;
    cout << "&socket_address_: " << &socket_address_ << endl;
    cout << "socket_address_ size: " << sizeof(socket_address_) << endl;
}
