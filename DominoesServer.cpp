/**
 * @file    ?.cpp
 * @author  Andrés Salinas Lima
 * @date    17/10/2019
 * @brief   Código de la clase ?:
 * // TODO: Add comments here
 */


#include "DominoesServer.h"
#include "Socket.h"
#include "constants.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


using std::cout;
using std::cin;
using std::cerr;
using std::endl;


void DominoesServer::start() {
    fd_set auxFDS; // Set de descriptores auxiliar para la funcion select()
    char receivedMessage[MSG_SIZE];
    bool end = false;
    int nReceived;


    // Inicializamos los conjuntos fd_set para la función select()
    FD_ZERO(&auxFDS); // Vacía el el set
    FD_ZERO(&readFDS); // Vacía el set
    FD_SET((unsigned) 0, &readFDS); // Añade stdin al set de descriptores de lectura
    FD_SET(serverSocket.getDescriptor(), &readFDS); // Añade el socket del
    // servidor al set de descriptores de lectura


    // Intercambio de mensajes
    do {
        auxFDS = readFDS; // Inicializamos auxFDS antes de llamar a select()

        // select() duerme el proceso hasta que haya datos disponibles en alguno
        // de los sockets del set
        if ((select(FD_SETSIZE, &auxFDS, nullptr, nullptr, nullptr)) < 0) {
            cerr << "Error en select(): " << strerror(errno) << endl;
            end = true;
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
                        handleClientCommunication(activeSocketD, receivedMessage);
                    }
                }
            }
        }
        fflush(stdout);
        fflush(stderr);
    } while (!end);
}

void DominoesServer::sendMessage(int destinationSocketD, const char *message) {
    char sentMessage[MSG_SIZE];

    bzero(sentMessage, sizeof(sentMessage));

    strcpy(sentMessage, message);

    if ((send(destinationSocketD, sentMessage, strlen(sentMessage), 0)) < 0)
        cerr << "Error al enviar mensaje al cliente " << destinationSocketD
             << ": " << strerror(errno) << endl;
}

void DominoesServer::handleUserInput() {
    // TODO
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
        // TODO: Comprobar que no superemos el limite de clientes
        // Tenemos un nuevo cliente

        FD_SET(newClientSocketD, &readFDS);

        cout << "Tenemos un nuevo cliente: " << newClientSocketD << endl;

        sendMessage(newClientSocketD, "Bienvenido");
    }
}

void DominoesServer::handleGoneClient(int goneClientSocketD) {
    cout << "El cliente " << goneClientSocketD << " nos ha abbandonado" << endl;

    close(goneClientSocketD);

    FD_CLR(goneClientSocketD, &readFDS);
}

void DominoesServer::handleClientCommunication(int clientSocketD,
                                               const char *receivedMessage) {
    cout << "Cliente " << clientSocketD << ": " << receivedMessage << endl;
}
