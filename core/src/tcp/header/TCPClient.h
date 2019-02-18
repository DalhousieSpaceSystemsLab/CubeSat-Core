#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

class TCPClient {
    public:
        TCPClient(int portno_){
            this->portno_ = portno_;
            sockfd_ = socket(AF_INET, SOCK_STREAM, 0);

            if (sockfd_ < 0)
                error("ERROR opening socket");

            server_ = gethostbyname("localhost");
            if (server_ == NULL) {
                fprintf(stderr,"ERROR, no such host\n");
                exit(0);
            }
            bzero((char *) &serv_addr_, sizeof(serv_addr_));
            serv_addr_.sin_family = AF_INET;
            bcopy((char *)server_->h_addr,
                (char *)&serv_addr_.sin_addr.s_addr,
                server_->h_length);
            serv_addr_.sin_port = htons(portno_);

            void bcopy(char *s1, char *s2, int length);
    
            if (connect(sockfd_, (struct sockaddr *) &serv_addr_,sizeof(serv_addr_)) < 0)
                error("ERROR connecting");

            printf("Please enter the message: ");
            bzero(buffer_,256);
            fgets(buffer_,255,stdin);
            n_ = write(sockfd_,buffer_,strlen(buffer_));
            if (n_ < 0)
                error("ERROR writing to socket");
            bzero(buffer_,256);
            n_ = read(sockfd_,buffer_,255);
            if (n_ < 0)
            error("ERROR reading from socket");
            printf("%s",buffer_);
        }
    private:
        int sockfd_;
        int portno_;
        int n_;
        struct sockaddr_in serv_addr_;
        struct hostent *server_;
        char buffer_[256];

        void error(const char *msg)
        {
            perror(msg);
            exit(1);
        }
};
