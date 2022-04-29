#include "Chat.h"
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatMessage::to_bin()
{
    alloc_data(MESSAGE_SIZE);

    memset(_data, 0, MESSAGE_SIZE);
    char *  _pointer = _data;
    memcpy(_pointer, &type, sizeof(uint8_t));
    _pointer += sizeof(uint8_t);
    memcpy(_pointer, &nick, sizeof(char) * 8);
    _pointer += sizeof(char) * 8;
    memcpy(_pointer, &message, sizeof(char) * 80);

    //Serializar los campos type, nick y message en el buffer _data
}

int ChatMessage::from_bin(char * bobj)
{
    alloc_data(MESSAGE_SIZE);

    memcpy(static_cast<void *>(_data), bobj, MESSAGE_SIZE);

    char *  _pointer = _data;
    memcpy(&type, _pointer, sizeof(uint8_t));
    _pointer += sizeof(uint8_t);
    memcpy(&nick, _pointer, sizeof(char) * 8);
    _pointer += sizeof(char) * 8;
    memcpy(&message, _pointer, sizeof(char) * 80);
    return 0;

    //Reconstruir la clase usando el buffer _data

    return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::do_messages()
{
    while (true)
    {
        std::unique_ptr<ChatMessage> msg;
        Socket * s;
        int rv = socket.recv(*msg, s);
        std::unique_ptr<Socket> sock(s);

        switch (msg->type)
        {
        case 0 :
            clients.push_back(std::move(sock));
            break;
        case 1 :
            for (int i = 0; i < clients.size(); i++){
                if (sock != clients[i]){
                    clients[i]->send(*msg, socket);
                };
            }
            break;
        case 2 :
            for (int index = 0; index < clients.size(); index++) {
                if (clients[index] == sock){
                    clients.erase(clients.begin() + index);
                    break;
                }
            }
            break;
        default:
            break;
        }
        /*
         * NOTA: los clientes están definidos con "smart pointers", es necesario
         * crear un unique_ptr con el objeto socket recibido y usar std::move
         * para añadirlo al vector
         */

        //Recibir Mensajes en y en función del tipo de mensaje
        // - LOGIN: Añadir al vector clients
        // - LOGOUT: Eliminar del vector clients
        // - MESSAGE: Reenviar el mensaje a todos los clientes (menos el emisor)


    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatClient::login()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGIN;

    socket.send(em, socket);
}

void ChatClient::logout()
{
    std::string msg;

    ChatMessage em(nick, msg);
    em.type = ChatMessage::LOGOUT;

    socket.send(em, socket);
}

void ChatClient::input_thread()
{
    while (true)
    {
        std::string msg;
        std::getline(std::cin, msg);
        ChatMessage em(nick, msg);
        em.type = ChatMessage::MESSAGE;
        // Leer stdin con std::getline
        // Enviar al servidor usando socket
    }
}

void ChatClient::net_thread()
{
    while(true)
    {
        //Recibir Mensajes de red
        //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
    }
}

