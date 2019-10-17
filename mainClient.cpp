/**
 * @file    mainClient.cpp
 * @author  Andrés Salinas Lima
 * @date    07/10/2019
 * @brief   Main del Cliente para servidor TCP: Envía y recibe cadenas de texto.
 */


#include "constants.h"
#include "Socket.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


using std::cout;
using std::cerr;
using std::cin;
using std::endl;


// TODO: Capturar señales (ctrl+c)
int main(int argc, char **argv) {
    // Se comprueban y procesan los argumentos pasados por la línea de comandos
    if (argc != 2) {
        cerr << "Uso del programa: " << argv[0] << " direcciónIPservidor" << endl;
        exit(EXIT_FAILURE);
    }
    char *serverIPAddress = argv[1];


    // Abrimos el socket y lo conectamos al servidor
    Socket socket;
    socket.prepareClientSocket(serverIPAddress);
    int socketDescriptor = socket.getDescriptor();


    // Inicializamos los conjuntos fd_set para la función select()
    fd_set readFDS, auxFDS; // Sets de descriptores para la funcion select()
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
            end = true;
        }

        if (FD_ISSET(socketDescriptor, &auxFDS)) {
            // Hemos recibido un mensaje del servidor en el socket

            bzero(receivedMessage, sizeof(receivedMessage));

            if ((recv(socketDescriptor, receivedMessage, sizeof(receivedMessage), 0)) < 0) {
                cerr << "Error al recibir mensaje del servidor: " << strerror(errno) << endl;
                socket.close();
                exit(EXIT_FAILURE);
            }

            cout << receivedMessage << endl;

            // TODO: Desconectar cliente si el servidor nos rechaza o algo
            if (strcmp(receivedMessage, "") == 0)
                end = true;

        } else if (FD_ISSET(0, &auxFDS)) {
            // El usuario ha tecleado un mensaje

            bzero(sentMessage, sizeof(sentMessage));

            cin.get(sentMessage, MSG_SIZE);
            cin.ignore();

            if ((send(socketDescriptor, sentMessage, sizeof(sentMessage), 0)) < 0) {
                cerr << "Error al enviar mensaje al servidor: " << strerror(errno) << endl;
                socket.close();
                exit(EXIT_FAILURE);
            }

            if (strcmp(sentMessage, "SALIR") == 0)
                end = true;
        }
        fflush(stdout);
        fflush(stderr);
    } while (!end);


    return EXIT_SUCCESS;
}
