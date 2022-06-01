#include <string.h>

#include "Serializable.h"
#include "Socket.h"
#include <unistd.h>

Socket::Socket(const char * address, const char * port):sd(-1)
{
    //Construir un socket de tipo AF_INET y SOCK_DGRAM usando getaddrinfo.
    //Con el resultado inicializar los miembros sd, sa y sa_len de la clase
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_flags    = AI_PASSIVE; //Devolver 0.0.0.0
    hints.ai_family   = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo* res;
    int rc = getaddrinfo(address, port, &hints, &res);

    if (rc != 0){
        std::cout << "Error: " << gai_strerror(rc) << "\n";
        freeaddrinfo(res);
        return;
    }

    sd = socket(res->ai_family, res->ai_socktype, 0);

    if (sd == -1){
        std::cout << "Error socket: " << errno <<  "\n";
        freeaddrinfo(res);
        return;
    }

    sa = *((struct sockaddr*)res->ai_addr);
    sa_len = res->ai_addrlen;
}

int Socket::recv(Serializable &obj, Socket * &sock)
{
    struct sockaddr sa;
    socklen_t sa_len = sizeof(struct sockaddr);

    char buffer[MAX_MESSAGE_SIZE];

    ssize_t bytes = ::recvfrom(cli, buffer, MAX_MESSAGE_SIZE, 0, &cliente, &cliente_len);

    if ( bytes <= 0 )
    {
        return -1;
    }

    if ( sock != 0 )
    {
        sock = new Socket(&sa, sa_len);
    }

    obj.from_bin(buffer);

    return 0;
}

int Socket::send(Serializable& obj)
{
    //Serializar el objeto
    //Enviar el objeto binario a sock usando el socket sd

    obj.to_bin();
    
    int st = sendto(cli, obj.data(), obj.size(), 0, &cliente, cliente_len);
    if (st == -1){
        std::string s;
        perror(s.c_str());
        std::cout << "Error send: " << errno <<  "\n";
        close(sd);
        return -1;
    }
    return 0;
}

bool operator== (const Socket &s1, const Socket &s2)
{
    //Comparar los campos sin_family, sin_addr.s_addr y sin_port
    //de la estructura sockaddr_in de los Sockets s1 y s2
    //Retornar false si alguno difiere
    const sockaddr_in so1 = (const sockaddr_in&)(s1.sa);
    const sockaddr_in so2 = (const sockaddr_in&)(s2.sa);
    return (so1.sin_family == so2.sin_family && so1.sin_addr.s_addr == so2.sin_addr.s_addr
    && so1.sin_port == so2.sin_port);
};

std::ostream& operator<<(std::ostream& os, const Socket& s)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];

    getnameinfo((struct sockaddr *) &(s.sa), s.sa_len, host, NI_MAXHOST, serv,
                NI_MAXSERV, NI_NUMERICHOST);

    os << host << ":" << serv;

    return os;
};

