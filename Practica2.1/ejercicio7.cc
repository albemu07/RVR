#include <sys/types.h>
#include <sys/socket.h>
#include <cstddef>
#include <string.h>
#include <netdb.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <vector>

// g++ -o ejercicio6 ejercicio6.cc -pthread

const int MAX_BUFFER = 80;
const int NUM_THREADS = 4;

class msgThreads
{
    public:
        msgThreads() {}
        msgThreads(int cli, int id, struct sockaddr client, socklen_t cliente_len) : cli_(cli), id_(id), client_(client), cliente_len_(cliente_len), th_(&msgThreads::recieveMsg, this) {
        }
        ~msgThreads() {
            std::cout << "Deataching thread: " << id_ << " " << std::this_thread::get_id() << "\n";
            th_.detach();
        }
        
        void recieveMsg() {
            bool con = true;
            while (con) {

                char buffer[MAX_BUFFER] = {};

                int bytes = recvfrom(cli_, buffer, MAX_BUFFER, 0, (struct sockaddr *) &client_,
                    &cliente_len_);
                buffer[bytes]='\0'; 
                if (bytes == -1){
                    std::cout << "Error recv: " << errno <<  "\n";
                    close(cli_);
                    return ;
                }
                else if (bytes == 0){
                    int st = sendto(cli_, "Client closed\n", 14, 0, (struct sockaddr *) &client_, cliente_len_);
                    if (st == -1){
                        std::cout << "Error send: " << errno <<  "\n";
                        close(cli_);
                        return;
                    }
                    std::cout << "Conexión terminada\n";
                    close(cli_);
                    con = false;
                }
                else{
                    int st = sendto(cli_, buffer, bytes, 0, (struct sockaddr *) &client_, cliente_len_);
                    if (st == -1){
                        std::cout << "Error send: " << errno <<  "\n";
                        close(cli_);
                        return;
                    }
                }
            }
            close(cli_);
            delete this;
        }
    private:
        int cli_;
        int id_;
        std::thread th_;
        struct sockaddr client_;
        socklen_t cliente_len_;
};


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

    int bd = bind(sd, (struct sockaddr *) res->ai_addr, res->ai_addrlen);

    freeaddrinfo(res);

    if (bd != 0){
        std::cout << "Error bind: " << errno <<  "\n";
        close(sd);
        return -1;
    }

    int lis = listen(sd, 16);

    if (lis != 0){
        std::cout << "Error listen: " << errno <<  "\n";
        close(sd);
        return -1;
    }

    std::vector<msgThreads*> clientThreads;
    
    int id = 0;

    while (true){
        struct sockaddr cliente;
        socklen_t cliente_len = sizeof(sockaddr);

        int cli = accept(sd, &cliente, &cliente_len);

        if (cli == -1){
            std::cout << "Error accept: " << errno <<  "\n";
            close(sd);
            close(cli);
            return -1;
        }

        
        char host[NI_MAXHOST];
        char serv[NI_MAXSERV];

        
        int ni = getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST,
            serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);

        if (ni != 0){
            std::cout << "Error getting name: " << gai_strerror(ni) << "\n";
            close(sd);
            close(cli);
            return -1; 
        }

        printf("Conexión desde Host:%s Puerto:%s\n",host, serv);

        new msgThreads(cli, id, cliente, cliente_len);
        id++;
    }
    close(sd);
    printf("Server Off");
    return 0;  
}