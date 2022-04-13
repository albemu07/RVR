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
    hints.ai_socktype = SOCK_DGRAM;

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

    bool con = true;
    while (con) {

        char buffer[MAX_BUFFER] = {};

        char host[NI_MAXHOST];
        char serv[NI_MAXSERV];

        struct sockaddr cliente;
        socklen_t cliente_len = sizeof(sockaddr);

        int bytes = recvfrom(sd, buffer, MAX_BUFFER, 0, (struct sockaddr *) &cliente,
            &cliente_len);
        buffer[bytes]='\0'; 
        
        if (bytes == -1){
            std::cout << "Error recv: " << errno <<  "\n";
            close(sd);
            return -1;
        }

        int ni = getnameinfo((struct sockaddr *) &cliente, cliente_len, host, NI_MAXHOST,
            serv, NI_MAXSERV, NI_NUMERICHOST|NI_NUMERICSERV);
        
        if (ni != 0){
            std::cout << "Error: " << gai_strerror(ni) << "\n";
            close(sd);
            return -1; 
        }

        printf("Conexión desde Host:%s Puerto:%s\n",host, serv);
        printf("\tMensaje(%i): %s\n", bytes, buffer);

        const char* inp = "";
        switch (buffer[0]){
            case 't':
                inp = "%r";
                break;
            case 'd':
                inp = "%F";
                break;
            case 'q':
                con = false;
                break;
            default:
                if (sendto(sd, "Comando no soportado\n", 21, 0,  &cliente, cliente_len) == -1){
                    std::cout << "Error send: " << errno <<  "\n";
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
                std::cout << "Comando no soportado " << buffer << "\n";
                break;
        }

        if (inp != ""){
            char output[MAX_BUFFER] = {};

            struct tm *t;
            time_t timeVar;

            time(&timeVar);
            t = localtime(&timeVar);

            int tam = strftime(output, MAX_BUFFER, inp, t);

            std::cout << output <<  "\n";

            int st = sendto(sd, output, tam, 0, (struct sockaddr *) &cliente, cliente_len);
            if (st == -1){
                std::cout << "Error send: " << errno <<  "\n";
                close(sd);
                return -1;
            }
        }
    }
    std::cout << "Saliendo...\n";
    close(sd);
    return 0;  
}