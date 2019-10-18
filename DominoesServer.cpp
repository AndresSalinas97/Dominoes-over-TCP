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
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;


DominoesServer::DominoesServer(const Socket &serverSocket) {
    this->serverSocket = serverSocket;
}

void DominoesServer::start() {
    fd_set auxFDS; // Set de descriptores auxiliar para la funcion select()
    char receivedMessage[MSG_SIZE];
    int nReceived;


    // Inicializamos los conjuntos fd_set para la función select()
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

    // TODO: Avisar a los clientes

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
    cin.ignore();

    if (input == "SALIR")
        end();
    else if (input == "STATS")
        printStats();
    else
        cout << "\t* Comando no reconocido *" << endl;
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
        // TODO: Comprobamos que no se supere el limite de clientes
        if (false) {
            sendMessage(newClientSocketD, "-ERR. Se ha superado el número de usuarios conectados");
        } else {
            // Tenemos un nuevo cliente
            // TODO

            FD_SET(newClientSocketD, &readFDS);

            cout << "\t* Nuevo cliente en socket " << newClientSocketD << " *" << endl;

            sendMessage(newClientSocketD, "+0k. Usuario conectado");
        }
    }
}

void DominoesServer::handleGoneClient(int goneClientSocketD) {
    // TODO
    cout << "\t* Cliente desconectado en socket " << goneClientSocketD << " *" << endl;

    close(goneClientSocketD);

    FD_CLR(goneClientSocketD, &readFDS);
}

void DominoesServer::handleClientCommunication(int clientSocketD,
                                               string receivedMessage) {
    // TODO
    cout << "Socket " << clientSocketD << ": " << receivedMessage << endl; // TODO: eliminar
}

void DominoesServer::printStats() {
    cout << "\t* STATS *" << endl;

    // TODO: Imprimir estadísticas (usuarios conectados, usuarios registrados, partidas en curso...)
    cerr << "\t\tSin implementar" << endl;
}
