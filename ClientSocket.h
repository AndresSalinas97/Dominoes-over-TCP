/**
 * @file    ClientSocket.h
 * @author  Andrés Salinas Lima
 * @date    23/10/2019
 * @brief   Declaración de la clase ClientSocket.
 */


#ifndef DOMINOES_OVER_TCP_CLIENTSOCKET_H
#define DOMINOES_OVER_TCP_CLIENTSOCKET_H


#include "Socket.h"


/**
 * @class ClientSocket: Abre, habilita y cierra sockets para el cliente.
 */
class ClientSocket : public Socket {
public:
    /**
     * Realiza las operaciones necesarias para que un socket cliente se
     * comunique con el servidor en la dirección especificada.
     */
    void prepare(const char *serverIPAddress);

private:
    /**
     * Solicita la conexión con el servidor.
     */
    void connect(struct sockaddr_in &serverAddress);
};


#endif //DOMINOES_OVER_TCP_CLIENTSOCKET_H
