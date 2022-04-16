#include <sys/types.h>
#include <sys/socket.h>
#include <cstddef>
#include <string.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

const int MAX_BUFFER = 80;

int main(int argc, char *argv[]){

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags    = AI_PASSIVE; //Devolver 0.0.0.0
    hints.ai_family   = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* res;
    int rc = getaddrinfo(argv[1], argv[2], &hints, &res);

    if (rc != 0){
        std::cout << "Error: " << gai_strerror(rc) << "\n";
        freeaddrinfo(res);
        return -1;
    }

    int sd = socket(res->ai_family, res->ai_socktype, 0);

    if (sd == -1){
        std::cout << "Error socket: " << errno <<  "\n";
        freeaddrinfo(res);
        return -1;
    }

    freeaddrinfo(res);

    
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    struct sockaddr_in server;
    socklen_t server_len = sizeof(sockaddr_in);

    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    server.sin_addr.s_addr = INADDR_ANY;

    int ser = connect(sd, (struct sockaddr *)&server, server_len);
    if (ser != 0){
        std::cout << "Error connect: " << errno <<  "\n";
        close(sd);
        return -1;
    }

    bool con = true;
    while(con){

        char* buffer = NULL;
        size_t len = MAX_BUFFER;
        size_t line = getline(&buffer, &len, stdin);

        if (buffer[0] == 'Q' && line == 2 ){
            con = false;
        }
        else{
            int st = sendto(sd, buffer, strlen(buffer), 0, (struct sockaddr *) &server, server_len);
            if (st == -1){
                std::cout << "Error send: " << errno <<  "\n";
                close(sd);
                return -1;
            }

            int bytes = recvfrom(sd, buffer, MAX_BUFFER, 0, (struct sockaddr *) &server,
            &server_len);
            if (bytes == -1){
                std::cout << "Error rcv: " << errno <<  "\n";
                close(sd);
                return -1;
            }
            if (bytes < MAX_BUFFER)
            {
                buffer[bytes] = '\0';
            }
            else
            {
                buffer[MAX_BUFFER-1] = '\0';
            }
            std::cout << buffer;
        }
    }
    close(sd);
    return 0;  
}