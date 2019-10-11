/**
 * @file    serverUtils.cpp
 * @author  Andrés Salinas Lima
 * @date    11/10/2019
 * @brief   Código de las funciones auxiliares del servidor.
 */


#include "constants.h"
#include "serverUtils.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>


using std::cout;
using std::cerr;
using std::endl;


int prepareServerSocket() {
    // Se abre el socket del servidor
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        cerr << "No se puede abrir el socket: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    // Para permitir que otro programa reutilice el puerto sin tener que esperar
    // el tiempo de espera (TIME_WAIT en el caso de TCP)
    int on = 1;
    if ((setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0) {
        cerr << "Error en setsockopt(): " << strerror(errno) << endl;
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Se rellenan los campos de la estructura de la dirección del servidor
    // (necesario para la llamada a la funcion bind())
    struct sockaddr_in server_address = {};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // Permite recibir
    // paquetes destinados a cualquiera de las interfaces

    // Asociamos el socket a un puerto (para decirle al SO que deseamos atender
    // a un determinado servicio, de forma que cuando llegue un mensaje por ese
    // servicio nos avise)
    if (bind(sd, (struct sockaddr *) &server_address,
             sizeof(server_address)) < 0) {
        cerr << "Error en bind(): " << strerror(errno) << endl;
        close(sd);
        exit(EXIT_FAILURE);
    }

    // Habilitamos el socket para que pueda recibir conexiones
    if (listen(sd, 1) < 0) {
        cerr << "Error en listen(): " << strerror(errno) << endl;
        close(sd);
        exit(EXIT_FAILURE);
    }

    return sd;
}


void handleUserInput()
{
    // TODO
}


void handleNewClient(int my_socket, fd_set &read_fds) {
    int new_client_socket;
    struct sockaddr_in client_address = {};
    socklen_t client_address_length = sizeof(client_address);
    char sent_message[MSG_SIZE];

    // Aceptamos la conexión obteniendo el socket del nuevo cliente
    if ((new_client_socket = accept(my_socket, (struct sockaddr *) &client_address,
                                    &client_address_length)) < 0) {
        cerr << "Error aceptando petición: " << strerror(errno) << endl;
    } else {
        // Tenemos un nuevo cliente
        // TODO: Comprobar que no superemos el limite de clientes
        FD_SET(new_client_socket, &read_fds);
        cout << "Tenemos un nuevo cliente: " << new_client_socket << endl;
        bzero(sent_message, sizeof(sent_message));
        strcpy(sent_message, "Bienvenido");
        if ((send(new_client_socket, sent_message, strlen(sent_message), 0)) < 0)
            cerr << "Error al enviar mensaje: " << strerror(errno) << endl;
    }
}


void handleGoneClient(int client_socket, fd_set &read_fds) {
    cout << "El cliente " << client_socket << " nos ha abbandonado" << endl;
    close(client_socket);
    FD_CLR(client_socket, &read_fds);
}


void handleClientCommunication(int active_socket, char* received_message) {
    cout << "Cliente " << active_socket << ": " << received_message << endl;
}
