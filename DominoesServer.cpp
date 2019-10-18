/**
 * @file    DominoesServer.cpp
 * @author  Andrés Salinas Lima
 * @date    17/10/2019
 * @brief   Código de la clase DominoesServer: Clase para gestionar el
 *          servidor del juego del Dominó.
 */


#include "DominoesServer.h"
#include "constants.h"

#include <iostream>
#include <cstring>
#include <vector>
#include <iterator>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::list;
using std::vector;


void DominoesServer::start() {
    char receivedMessage[MSG_SIZE];
    int nReceived;


    // Mensajes de inicio:
    printHelp();
    cout << "\t* Servidor iniciado *" << endl;


    // Inicializamos los conjuntos fd_set para la función select()
    fd_set auxFDS; // Set de descriptores auxiliar para la funcion select()
    FD_ZERO(&auxFDS); // Vacía el el set
    FD_ZERO(&readFDS); // Vacía el set
    FD_SET((unsigned) 0, &readFDS); // Añade stdin al set de descriptores de lectura
    FD_SET(serverSocket.getDescriptor(), &readFDS); // Añade el socket del
    // servidor al set de descriptores de lectura


    // Intercambio de mensajes
    while (true) {
        auxFDS = readFDS; // Inicializamos auxFDS antes de llamar a select()

        // select() duerme el proceso hasta que haya datos disponibles en alguno
        // de los sockets del set
        if ((select(FD_SETSIZE, &auxFDS, nullptr, nullptr, nullptr)) < 0) {
            cerr << "Error en select(): " << strerror(errno) << endl;
            break;
        }

        for (int activeSocketD = 0; activeSocketD < FD_SETSIZE; activeSocketD++) {
            // Recorremos todos los descriptores posibles
            if (FD_ISSET(activeSocketD, &auxFDS)) {
                // Tenemos nuevos datos en el socket i
                if (activeSocketD == 0) {
                    // El usuario ha tecleado un mensaje
                    handleUserInput();
                } else if (activeSocketD == serverSocket.getDescriptor()) {
                    // Tenemos nuevos datos en el socket del servidor lo que
                    // siginifica que tenemos un nuevo cliente
                    handleNewClient();
                } else {
                    // Tenemos nuevos datos en el socket de algún cliente
                    bzero(receivedMessage, sizeof(receivedMessage));
                    nReceived = recv(activeSocketD, receivedMessage,
                                     sizeof(receivedMessage), 0);

                    if (nReceived < 0) {
                        cerr << "Error al recibir mensaje del cliente "
                             << activeSocketD << ": " << strerror(errno) << endl;
                    } else if (nReceived == 0) {
                        // El cliente nos ha abandonado
                        handleGoneClient(activeSocketD);
                    } else {
                        // El cliente nos envía un mensaje
                        handleClientCommunication(activeSocketD, string(receivedMessage));
                    }
                }
            }
        }

        fflush(stdout);
        fflush(stderr);
    }


    // El servidor termina cerrando el socket
    serverSocket.close();
}

void DominoesServer::end() {
    cout << "\t* El servidor se cerrará después de avisar a los clientes *" << endl;

    for (list<Client>::const_iterator it = clients.begin(); it != clients.end(); it++)
        sendMessage(it->getSocketDescriptor(), "*INFO. El servidor va a cerrar");

    clients.empty();
    // TODO: Vaciar lista partidas

    serverSocket.close();

    exit(EXIT_SUCCESS);
}

void DominoesServer::sendMessage(int destinationSocketD, const char *message) {
    // Para asegurar que no se sobrepasa MSG_SIZE
    char sentMessage[MSG_SIZE];
    bzero(sentMessage, sizeof(sentMessage));
    strcpy(sentMessage, message);

    if ((send(destinationSocketD, sentMessage, strlen(sentMessage), 0)) < 0)
        cerr << "Error al enviar mensaje al cliente " << destinationSocketD
             << ": " << strerror(errno) << endl;
}

void DominoesServer::handleUserInput() {
    string input;

    cin >> input;
    cin.clear();
    cin.ignore();

    if (input == "SALIR")
        end();
    else if (input == "STATS")
        printStats();
    else if (input == "AYUDA")
        printHelp();
    else
        cout << "\t* Comando no válido *" << endl;
}

void DominoesServer::handleNewClient() {
    int newClientSocketD;
    struct sockaddr_in clientAddress = {};
    socklen_t clientAddressLength = sizeof(clientAddress);

    // Aceptamos la conexión obteniendo el socket del nuevo cliente
    if ((newClientSocketD = accept(serverSocket.getDescriptor(),
                                   (struct sockaddr *) &clientAddress,
                                   &clientAddressLength)) < 0) {
        cerr << "Error en accept(): " << strerror(errno) << endl;
    } else {
        if (clients.size() >= MAX_CLIENTS) {
            sendMessage(newClientSocketD, "-ERR. Se ha superado el número de "
                                          "usuarios conectados");
        } else {
            // Tenemos un nuevo cliente

            // Añadimos el nuevo cliente a la lista de clientes
            Client newClient(newClientSocketD);
            clients.push_back(newClient);

            FD_SET(newClientSocketD, &readFDS);

            cout << "\t* Nuevo cliente en socket " << newClientSocketD << " *" << endl;

            sendHelp(newClientSocketD);

            sendMessage(newClientSocketD, "+OK. Usuario conectado");
        }
    }
}

void DominoesServer::handleGoneClient(int goneClientSocketD) {
    cout << "\t* Cliente desconectado en socket " << goneClientSocketD << " *" << endl;

    removeClient(goneClientSocketD);
}

void DominoesServer::removeClient(int goneClientSocketD) {
    // TODO: Si el cliente estaba jugando, notificar a su oponente, poner a su oponente en not playing y eliminar la partida

    // Se elimina de la lista de clientes
    for (list<Client>::const_iterator it = clients.begin(); it != clients.end(); it++)
        if (it->getSocketDescriptor() == goneClientSocketD) {
            clients.erase(it);
            break;
        }

    // Se cierra su socket
    close(goneClientSocketD);

    // Se elimina su socket de readFDS
    FD_CLR(goneClientSocketD, &readFDS);
}

void DominoesServer::handleClientCommunication(int clientSocketD,
                                               const string &receivedMessage) {
    cout << "Socket " << clientSocketD << ": \"" << receivedMessage << "\"" << endl; // TODO: eliminar

    // Separamos receivedMessage en palabras
    std::istringstream iss(receivedMessage);
    vector<string> tokens{std::istream_iterator<string>{iss}, std::istream_iterator<string>{}};

    if (tokens.empty()) {
        sendMessage(clientSocketD, "-ERR. Mensaje no válido");
        return;
    }

    if (tokens[0] == "USUARIO") {
        if (tokens.size() != 2)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handleUsuarioCommand(clientSocketD, tokens[1]);

    } else if (tokens[0] == "PASSWORD") {
        if (tokens.size() != 2)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handlePasswordCommand(clientSocketD, tokens[1]);

    } else if (tokens[0] == "REGISTRO") {
        if (tokens.size() != 5 || tokens[1] != "-d" || tokens[3] != "-p")
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handleRegistroCommand(clientSocketD, tokens[2], tokens[4]);

    } else if (tokens[0] == "INICIAR-PARTIDA") {
        if (tokens.size() != 1)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handleIniciarPartidaCommand(clientSocketD);

    } else if (tokens[0] == "COLOCAR-FICHA") {
        if (tokens.size() != 2)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handleColocarFichaCommand(clientSocketD, tokens[1]);

    } else if (tokens[0] == "PASO-TURNO") {
        if (tokens.size() != 1)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handlePasoTurnoCommand(clientSocketD);

    } else if (tokens[0] == "ROBAR-FICHA") {
        if (tokens.size() != 1)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handleRobarFichaCommand(clientSocketD);

    } else if (tokens[0] == "AYUDA") {
        if (tokens.size() != 1)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            sendHelp(clientSocketD);

    } else if (tokens[0] == "SALIR") {
        if (tokens.size() != 1)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handleSalirCommand(clientSocketD);

    } else {
        sendMessage(clientSocketD, "-ERR. Comando no válido");
    }
}

void DominoesServer::printStats() {
    cout << "\t* ESTADÍSTICAS *" << endl;

    cout << "\t\tClientes conectados: " << clients.size() << endl;
    cout << "\t\tPartidas en curso: " << "?" << endl; // TODO
    cout << "\t\tClientes registrados: " << "?" << endl; // TODO
}

void DominoesServer::printHelp() {
    cout << "\t* COMANDOS DISPONIBLES *" << endl;
    cout << "\t\t- STATS: Mostrar estadísticas" << endl;
    cout << "\t\t- SALIR: Salir" << endl;
    cout << "\t\t- AYUDA: Mostrar esta ayuda" << endl;
}

void DominoesServer::handleUsuarioCommand(int clientSocketD, const string &username) {
    // TODO
    sendMessage(clientSocketD, "*INFO. SIN IMPLEMENTAR");
}

void DominoesServer::handlePasswordCommand(int clientSocketD, const string &password) {
    // TODO
    sendMessage(clientSocketD, "*INFO. SIN IMPLEMENTAR");
}

void DominoesServer::handleRegistroCommand(int clientSocketD, const string &username, const string &password) {
    // TODO
    sendMessage(clientSocketD, "*INFO. SIN IMPLEMENTAR");
}

void DominoesServer::handleIniciarPartidaCommand(int clientSocketD) {
    // TODO
    sendMessage(clientSocketD, "*INFO. SIN IMPLEMENTAR");
}

void DominoesServer::handleColocarFichaCommand(int clientSocketD, const string &dominoAndSide) {
    // TODO
    sendMessage(clientSocketD, "*INFO. SIN IMPLEMENTAR");
}

void DominoesServer::handlePasoTurnoCommand(int clientSocketD) {
    // TODO
    sendMessage(clientSocketD, "*INFO. SIN IMPLEMENTAR");
}

void DominoesServer::handleRobarFichaCommand(int clientSocketD) {
    // TODO
    sendMessage(clientSocketD, "*INFO. SIN IMPLEMENTAR");
}

void DominoesServer::sendHelp(int clientSocketD) {
    // TODO
    sendMessage(clientSocketD, "*INFO. SIN IMPLEMENTAR");
}

void DominoesServer::handleSalirCommand(int clientSocketD) {
    sendMessage(clientSocketD, "+OK. Desconexión procesada");

    cout << "\t* Cliente abandona en socket " << clientSocketD << " *" << endl;

    removeClient(clientSocketD);
}
