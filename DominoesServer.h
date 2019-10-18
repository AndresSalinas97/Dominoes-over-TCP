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


/**
 * Clase para gestionar el servidor del juego del Dominó.
 */
class DominoesServer {
// TODO: Documentar miembros clase
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

    /**
     * Acciones a realizar cuando un cliente se desconecta.
     */
    void handleGoneClient(int goneClientSocketD);

    void removeClient(int goneClientSocketD);

    void handleClientCommunication(int clientSocketD, const string &receivedMessage);

    void printStats();

    static void printHelp();

    void handleUsuarioCommand(int clientSocketD, const string &username);

    void handlePasswordCommand(int clientSocketD, const string &password);

    void handleRegistroCommand(int clientSocketD, const string &username, const string &password);

    void handleIniciarPartidaCommand(int clientSocketD);

    void handleColocarFichaCommand(int clientSocketD, const string &dominoAndSide);

    void handlePasoTurnoCommand(int clientSocketD);

    void handleRobarFichaCommand(int clientSocketD);

    void sendHelp(int clientSocketD);

    void handleSalirCommand(int clientSocketD);

    Client & findClient(int clientSocketD);
};


#endif //DOMINOES_OVER_TCP_DOMINOESSERVER_H
