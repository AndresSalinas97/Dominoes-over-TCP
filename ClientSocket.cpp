/**
 * @file    ClientSocket.cpp
 * @author  Andrés Salinas Lima
 * @date    23/10/2019
 * @brief   Código de la clase ClientSocket.
 */


#include "ClientSocket.h"
#include "constants.h"

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


using namespace std;

void ClientSocket::prepare(const char *serverIPAddress) {
    // Se rellenan los campos de la estructura de la dirección del servidor
    struct sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = inet_addr(serverIPAddress);

    connect(serverAddress);
}

void ClientSocket::connect(struct sockaddr_in &serverAddress) {
    socklen_t serverAddressLength = sizeof(serverAddress);

    if (::connect(getDescriptor(), (struct sockaddr *) &serverAddress,
                  serverAddressLength) < 0) {
        cerr << "Error al conectar con el servidor: " << strerror(errno) << endl;
        close();
        exit(EXIT_FAILURE);
    }
}
