/**
 * @file    ServerSocket.cpp
 * @author  Andrés Salinas Lima
 * @date    23/10/2019
 * @brief   Código de la clase ServerSocket.
 */


#include "ServerSocket.h"
#include "constants.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>


using namespace std;


void ServerSocket::prepare() {
    setReuseAddrOpt();

    bind();

    listen();
}

void ServerSocket::setReuseAddrOpt() {
    int on = 1;
    if ((::setsockopt(getDescriptor(), SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0) {
        cerr << "Error en setsockopt(): " << strerror(errno) << endl;
        close();
        exit(EXIT_FAILURE);
    }
}

void ServerSocket::bind() {
    // Se rellenan los campos de la estructura de la dirección del servidor
    // (necesario para la llamada a la función bind())
    struct sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Permite recibir
    // paquetes destinados a cualquiera de las interfaces

    if (::bind(getDescriptor(), (struct sockaddr *) &serverAddress,
               sizeof(serverAddress)) < 0) {
        cerr << "Error en bind(): " << strerror(errno) << endl;
        close();
        exit(EXIT_FAILURE);
    }
}

void ServerSocket::listen() {
    if (::listen(getDescriptor(), 1) < 0) {
        cerr << "Error en listen(): " << strerror(errno) << endl;
        close();
        exit(EXIT_FAILURE);
    }
}
