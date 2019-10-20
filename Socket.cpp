/**
 * @file    Socket.cpp
 * @author  Andrés Salinas Lima
 * @date    17/10/2019
 * @brief   Código de la clase Socket: Abre, habilita y cierra sockets
 *          para el cliente y el servidor.
 */


#include "Socket.h"
#include "constants.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using std::cerr;
using std::endl;


Socket::Socket() {
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0) {
        cerr << "No se puede abrir el socket: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

void Socket::close() {
    if ((::close(socketDescriptor)) < 0) {
        cerr << "No se puede cerrar el socket: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
}

void Socket::setsockopt() {
    int on = 1;
    if ((::setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0) {
        cerr << "Error en setsockopt(): " << strerror(errno) << endl;
        close();
        exit(EXIT_FAILURE);
    }
}

void Socket::bind() {
    // Se rellenan los campos de la estructura de la dirección del servidor
    // (necesario para la llamada a la función bind())
    struct sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Permite recibir
    // paquetes destinados a cualquiera de las interfaces

    if (::bind(socketDescriptor, (struct sockaddr *) &serverAddress,
               sizeof(serverAddress)) < 0) {
        cerr << "Error en bind(): " << strerror(errno) << endl;
        close();
        exit(EXIT_FAILURE);
    }
}

void Socket::listen() {
    if (::listen(socketDescriptor, 1) < 0) {
        cerr << "Error en listen(): " << strerror(errno) << endl;
        close();
        exit(EXIT_FAILURE);
    }
}

void Socket::prepareServerSocket() {
    setsockopt();

    bind();

    listen();
}

void Socket::connect(struct sockaddr_in &serverAddress) {
    socklen_t serverAddressLength = sizeof(serverAddress);

    if (::connect(socketDescriptor, (struct sockaddr *) &serverAddress,
                  serverAddressLength) < 0) {
        cerr << "Error al conectar con el servidor: " << strerror(errno) << endl;
        close();
        exit(EXIT_FAILURE);
    }
}

void Socket::prepareClientSocket(const char *serverIPAddress) {
    // Se rellenan los campos de la estructura de la dirección del servidor
    struct sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = inet_addr(serverIPAddress);

    connect(serverAddress);
}
