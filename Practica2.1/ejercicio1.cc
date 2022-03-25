#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char *argv[]){

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_sockettype = SOCK_STREAM;
    hins.ai_flags = AI_PASSIVE; 

    struct addrinfo* res;
    if (argc > 0) {
        int suc = getaddrinfo(argv[1], NULL, &hints, &res);
        if (suc != 0)
            return -1;
        for (struct addrinfo* rp = res; i rp != nullptr; rp = rp->ai_next){
            getnameinfo();
        }

    }



    return 0;
}