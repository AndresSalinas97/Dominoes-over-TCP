/**
 * @file    mainServer.cpp
 * @author  Andrés Salinas Lima
 * @date    07/10/2019
 * @brief   Servidor TCP para el juego del Dominó: Gestiona clientes, usuarios,
 *          partidas y la lógica del juego.
 */


#include "constants.h"
#include "serverUtils.h"
#include "Client.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>


using std::cout;
using std::cerr;
using std::cin;
using std::endl;


// TODO: Capturar señales (ctrl+c)
int main() {
    int my_socket;
    fd_set read_fds, aux_fds; // Sets de descriptores para la funcion select()
    char received_message[MSG_SIZE];
    bool end = false;
    int n_received;


    // Abrimos el socket y lo habilitamos para que pueda recibir conexiones
    my_socket = prepareServerSocket();


    // Inicializamos los conjuntos fd_set para la función select()
    FD_ZERO(&aux_fds); // Vacía el el set
    FD_ZERO(&read_fds); // Vacía el set
    FD_SET((unsigned) 0, &read_fds); // Añade stdin al set de descriptores de lectura
    FD_SET(my_socket, &read_fds); // Añade el socket al set de descriptores de lectura


    // Intercambio de mensajes
    do {
        aux_fds = read_fds; // Inicializamos aux_fds antes de llamar a select()

        // select() duerme el proceso hasta que haya datos disponibles en alguno
        // de los sockets del set
        if ((select(FD_SETSIZE, &aux_fds, nullptr, nullptr, nullptr)) < 0) {
            cerr << "Error en select: " << strerror(errno) << endl;
            end = true;
        }

        for (int active_socket = 0; active_socket < FD_SETSIZE; active_socket++) {
            // Recorremos todos los descriptores posibles
            if (FD_ISSET(active_socket, &aux_fds)) {
                // Tenemos nuevos datos en el socket i
                if (active_socket == 0) {
                    // El usuario ha tecleado un mensaje
                    handleUserInput();
                } else if (active_socket == my_socket) {
                    // Tenemos nuevos datos en el socket del servidor
                    handleNewClient(my_socket, read_fds);
                } else {
                    // Tenemos nuevos datos en el socket de algún cliente
                    bzero(received_message, sizeof(received_message));
                    n_received = recv(active_socket, received_message,
                            sizeof(received_message), 0);

                    if (n_received < 0) {
                        cerr << "Error al recibir mensaje: " << strerror(errno) << endl;
                    } else if (n_received == 0) {
                        // El cliente nos ha abandonado
                        handleGoneClient(active_socket, read_fds);
                    } else {
                        // El cliente nos envía un mensaje
                        handleClientCommunication(active_socket, received_message);
                    }
                }
            }
        }
        fflush(stdout);
        fflush(stderr);
    } while (!end);


    close(my_socket);
    return EXIT_SUCCESS;
}
