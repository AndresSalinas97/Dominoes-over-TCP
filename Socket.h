/**
 * @file    Socket.h
 * @author  Andrés Salinas Lima
 * @date    17/10/2019
 * @brief   Declaración de la clase Socket.
 */


#ifndef DOMINOES_OVER_TCP_SOCKET_H
#define DOMINOES_OVER_TCP_SOCKET_H


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>


using namespace std;


/**
 * @class Socket: Base para las clases ServerSocket y ClientSocket.
 */
class Socket {
protected:
    /**
     * Constructor: Se abre el socket.
     * (Protegido para evitar que esta clase sea instanciada)
     */
    inline Socket() {
        socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
        if (socketDescriptor < 0) {
            cerr << "No se puede abrir el socket: " << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
    }

public:
    /**
     * Destructor: Se cierra el socket.
     */
    inline ~Socket() {
        this->close();
    }

    /**
     * Devuelve el descriptor del socket.
     */
    inline int getDescriptor() const {
        return socketDescriptor;
    }

    /**
     * Se cierra el socket.
     */
    inline void close() {
        if ((::close(socketDescriptor)) < 0) {
            cerr << "No se puede cerrar el socket: " << strerror(errno) << endl;
            exit(EXIT_FAILURE);
        }
    }

private:
    int socketDescriptor; /** Descriptor del socket. */
};


#endif //DOMINOES_OVER_TCP_SOCKET_H
