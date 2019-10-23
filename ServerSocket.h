/**
 * @file    ServerSocket.h
 * @author  Andrés Salinas Lima
 * @date    23/10/2019
 * @brief   Declaración de la clase ServerSocket.
 */

#ifndef DOMINOES_OVER_TCP_SERVERSOCKET_H
#define DOMINOES_OVER_TCP_SERVERSOCKET_H


#include "Socket.h"


/**
 * @class ServerSocket: Abre, habilita y cierra sockets para el servidor.
 */
class ServerSocket : public Socket {
public:
    /**
     * Realiza las operaciones necesarias para que un socket servidor empiece a
     * recibir conexiones.
     */
    void prepare();

private:
    /**
     * Establece las opciones del socket para permitir que otro programa
     * reutilice el puerto sin tener que esperar el tiempo de espera (TIME_WAIT
     * en el caso de TCP).
     */
    void setReuseAddrOpt();

    /**
     * Asocia el socket a un puerto (para decirle al SO que deseamos atender
     * a un determinado servicio, de forma que cuando llegue un mensaje por ese
     * servicio nos avise).
     */
    void bind();

    /**
     * Habilita el socket para que pueda recibir conexiones.
     */
    void listen();
};


#endif //DOMINOES_OVER_TCP_SERVERSOCKET_H
