/**
 * @file    DominoesServer.h
 * @author  Andrés Salinas Lima
 * @date    17/10/2019
 * @brief   Declaración de la clase DominoesServer: Clase para gestionar el
 *          servidor del juego del Dominó.
 */


#ifndef DOMINOES_OVER_TCP_DOMINOESSERVER_H
#define DOMINOES_OVER_TCP_DOMINOESSERVER_H


#include "Socket.h"
#include "Client.h"

#include <sys/socket.h>
#include <string>
#include <list>


using std::string;
using std::list;


class DominoesServer {
public:
    inline explicit DominoesServer(const Socket &serverSocket) : serverSocket(serverSocket) {};

    void start();

    void end();


private:
    Socket serverSocket;  // Objeto Socket del servidor
    fd_set readFDS{};     // Set de descriptores para la funcion select()
    list<Client> clients; // Lista de clientes conectados

    static void sendMessage(int destinationSocketD, const char *message);

    void handleUserInput();

    void handleNewClient();

    void handleGoneClient(int goneClientSocketD);

    void handleClientCommunication(int clientSocketD, const string &receivedMessage);

    void printStats();
};


#endif //DOMINOES_OVER_TCP_DOMINOESSERVER_H
