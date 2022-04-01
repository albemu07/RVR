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
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; 

    struct addrinfo* res;
    if (argc > 0) {
        int suc = getaddrinfo(argv[1], NULL, &hints, &res);
        if (suc != 0)
            return -1;
        for (struct addrinfo* rp = res; rp != nullptr; rp = rp->ai_next){
            char host[NI_MAXHOST];
            char serv[NI_MAXSERV];
            getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST,
                serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
            
            std::cout << "Host: " << host << " " << serv << "\n";
        }
    }

    return 0;
}