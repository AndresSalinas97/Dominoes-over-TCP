/**
 * @file    mainClient.cpp
 * @author  Andrés Salinas Lima
 * @date    07/10/2019
 * @brief   Main del Cliente para servidor TCP: Envía y recibe cadenas de texto.
 */


#include "constants.h"
#include "ClientSocket.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <csignal>
#include <unistd.h>


using namespace std;


int socketDescriptor; // Para poderlo usar en signalHandler()


void signalHandler(int);


int main(int argc, char **argv) {
    // Se comprueban y procesan los argumentos pasados por la línea de comandos
    if (argc != 2) {
        cerr << "Uso del programa: " << argv[0] << " direcciónIPServidor" << endl;
        exit(EXIT_FAILURE);
    }
    char *serverAddress = argv[1];


    // Abrimos el socket y lo conectamos al servidor
    ClientSocket socket;
    socket.prepare(serverAddress);
    socketDescriptor = socket.getDescriptor();


    // Capturamos la señal SIGINT (Ctrl+c)
    signal(SIGINT, signalHandler);


    // Inicializamos los conjuntos fd_set para la función select()
    fd_set readFDS, auxFDS; // Sets de descriptores para la función select()
    FD_ZERO(&auxFDS); // Vacía el el set
    FD_ZERO(&readFDS); // Vacía el set
    FD_SET(0, &readFDS); // Añade stdin al set de descriptores de lectura
    FD_SET(socketDescriptor, &readFDS); // Añade el socket al set de descriptores de lectura


    // Intercambio de mensajes con el servidor
    char receivedMessage[MSG_SIZE], sentMessage[MSG_SIZE];
    bool end = false;
    do {
        auxFDS = readFDS; // Inicializamos auxFDS antes de llamar a select()

        // select() duerme el proceso hasta que haya datos disponibles en alguno
        // de los sockets del set
        if ((select(socketDescriptor + 1, &auxFDS, nullptr, nullptr, nullptr)) < 0) {
            cerr << "Error en select(): " << strerror(errno) << endl;
            socket.close();
            exit(EXIT_FAILURE);
        }

        if (FD_ISSET(socketDescriptor, &auxFDS)) {
            // Hemos recibido un mensaje del servidor en el socket

            bzero(receivedMessage, sizeof(receivedMessage));

            int nReceived = recv(socketDescriptor, receivedMessage, sizeof(receivedMessage), 0);
            if (nReceived < 0) {
                cerr << "Error al recibir mensaje del servidor: " << strerror(errno) << endl;
                socket.close();
                exit(EXIT_FAILURE);
            } else if (nReceived == 0) {
                cout << "\t* El servidor ha terminado la conexión *" << endl;
                end = true;
            } else {
                cout << receivedMessage;

                if (strcmp(receivedMessage, "-ERR. Se ha superado el número de "
                                            "usuarios conectados") == 0)
                    end = true;
            }
        } else if (FD_ISSET(0, &auxFDS)) {
            // El usuario ha tecleado un mensaje

            bzero(sentMessage, sizeof(sentMessage));

            cin.get(sentMessage, MSG_SIZE);
            cin.clear();
            cin.ignore();

            if ((send(socketDescriptor, sentMessage, sizeof(sentMessage), 0)) < 0) {
                cerr << "Error al enviar mensaje al servidor: " << strerror(errno) << endl;
                socket.close();
                exit(EXIT_FAILURE);
            }
        }

        fflush(stdout);
        fflush(stderr);
    } while (!end);

    return EXIT_SUCCESS;
}


void signalHandler(int) {
    cout << endl << "\t* Se fuerza la salida *" << endl;

    close(socketDescriptor);

    exit(EXIT_SUCCESS);
}
