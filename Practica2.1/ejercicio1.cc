#include <sys/types.h>
#include <sys/socket.h>
#include <cstddef>
#include <string.h>
#include <netdb.h>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[]){

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = AI_PASSIVE; 

    struct addrinfo* res;
    if (argc > 0) {
        int suc = getaddrinfo(argv[1], NULL, &hints, &res);
        if (suc != 0){
            std::cout << "Error: " << gai_strerror(suc) << "\n";
            freeaddrinfo(res);
            return -1;
        }
        for (struct addrinfo* rp = res; rp != nullptr; rp = rp->ai_next){
            char host[NI_MAXHOST];
            getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST,
                NULL, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
            
            std::cout << "Host: " << host << " " << rp->ai_family 
            << " " << rp->ai_socktype <<"\n";
        }
    }
    freeaddrinfo(res);
    return 0;
}