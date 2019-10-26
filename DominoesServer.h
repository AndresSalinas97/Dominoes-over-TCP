/**
 * @file    DominoesServer.h
 * @author  Andrés Salinas Lima
 * @date    17/10/2019
 * @brief   Declaración de la clase DominoesServer.
 */


#ifndef DOMINOES_OVER_TCP_DOMINOESSERVER_H
#define DOMINOES_OVER_TCP_DOMINOESSERVER_H


#include "Socket.h"
#include "User.h"
#include "UsersManager.h"

#include <sys/socket.h>
#include <string>
#include <list>


using namespace std;


/**
 * @class DominoesServer: Clase para gestionar el servidor del juego del Dominó.
 */
class DominoesServer {
public:
    /**
     * Constructor: Crea un nuevo servidor que escuchará en el socket introducido.
     */
    inline explicit DominoesServer(const Socket &serverSocket)
            : serverSocket(serverSocket) {};

    /**
     * Inicia el servidor.
     * Función principal del servidor.
     */
    void start();

    /**
     * Cierra el servidor después de avisar a los clientes.
     */
    void end();

private:
    Socket serverSocket;                /** Objeto Socket del servidor. */
    fd_set readFDS{};                   /** Set de descriptores para la función select(). */
    UsersManager usersManager;          /** Mánager de usuarios. */
    list<DominoesBoard> dominoesBoards; /** Tableros de dominó donde los usuarios juegan. */

    /**
     * Envía el mensaje introducido al cliente con el socketD introducido.
     * @warning Se producirá un error si no hay un cliente conectado en el
     * socketD introducido.
     */
    static void sendMessage(int destinationSocketD, const char *message);

    /**
     * Acciones a realizar cuando se introduce texto por la entrada estándar
     * (stdin) del programa servidor.
     */
    void handleUserInput();

    /**
     * Acciones a realizar cuando un nuevo cliente se conecta al servidor.
     */
    void handleNewClient();

    /**
     * Acciones a realizar cuando un cliente se desconecta.
     */
    void handleGoneClient(int goneClientSocketD);

    /**
     * Acciones a realizar para eliminar a un cliente cuando este se desconecta.
     */
    void removeClient(int goneClientSocketD);

    /**
     * Elimina el tablero introducido de la lista de tableros.
     */
    void removeDominoesBoard(DominoesBoard *dominoesBoard);

    /**
     * Acciones a realizar cuando se recibe un mensaje de un cliente.
     */
    void handleClientCommunication(int clientSocketD, const string &receivedMessage);

    /**
     * Imprime estadísticas en la salida estándar (stdout) del programa servidor.
     */
    void printStats() const;

    /**
     * Imprime los comandos disponibles en la salida estándar (stdout) del
     * programa servidor.
     */
    static void printHelp();

    /**
     * Acciones a realizar cuando un cliente envía el comando USUARIO.
     */
    void handleUsuarioCommand(int clientSocketD, const string &username);

    /**
     * Acciones a realizar cuando un cliente envía el comando PASSWORD.
     */
    void handlePasswordCommand(int clientSocketD, const string &password);

    /**
     * Acciones a realizar cuando un cliente envía el comando REGISTRO.
     */
    void handleRegistroCommand(int clientSocketD, const string &username,
                               const string &password);

    /**
     * Acciones a realizar cuando un cliente envía el comando INICIAR-PARTIDA.
     */
    void handleIniciarPartidaCommand(int clientSocketD);

    /**
     * Acciones a realizar cuando un cliente envía el comando COLOCAR-FICHA.
     */
    void handleColocarFichaCommand(int clientSocketD, const string &dominoAndSide);

    /**
     * Acciones a realizar cuando un cliente envía el comando PASO-TURNO.
     */
    void handlePasoTurnoCommand(int clientSocketD);

    /**
     * Acciones a realizar cuando un cliente envía el comando ROBAR-FICHA.
     */
    void handleRobarFichaCommand(int clientSocketD);

    /**
     * Envía al cliente con el socketD introducido mensajes con los comandos
     * que tiene disponibles.
     */
    void sendHelp(int clientSocketD);

    /**
     * Envía al usuario especificado sus fichas.
     */
    static void sendTiles(User &user);

    /**
     * Envía al al usuario especificado las fichas del tablero.
     */
    static void sendBoard(User &user);

    /**
     * Acciones a realizar cuando un cliente envía el comando SALIR.
     */
    void handleSalirCommand(int clientSocketD);

    /**
     * Comprueba si hay ganadores, y realiza las acciones necesarias en caso de
     * que los haya.
     */
    bool checkWinners(User *user, User *opponent, DominoesBoard *dominoesBoard);
};


#endif //DOMINOES_OVER_TCP_DOMINOESSERVER_H
