/**
 * @file    Socket.cpp
 * @author  Andrés Salinas Lima
 * @date    17/10/2019
 * @brief   Declaración de la clase Socket: Abre, habilita y cierra
 *          sockets para el cliente y el servidor.
 */


#ifndef DOMINOES_OVER_TCP_SOCKET_H
#define DOMINOES_OVER_TCP_SOCKET_H


/**
 * Clase Socket: Abre, habilita y cierra sockets para el cliente y el servidor.
 */
class Socket {
public:
    /**
     * Constructor: Se abre el socket.
     */
    Socket();

    /**
     * Destructor: Se cierra el socket.
     */
    inline ~Socket() { this->close(); }

    /**
     * Devuelve el descriptor del socket.
     */
    inline int getDescriptor() { return socketDescriptor; }

    /**
     * Realiza las operaciones necesarias para que un socket servidor empiece a
     * recibir conexiones.
     */
    void prepareServerSocket();

    /**
     * Realiza las operaciones necesarias para que un socket cliente se
     * comunique con el servidor en la dirección especificada.
     */
    void prepareClientSocket(const char *serverIPAddress);

    /**
     * Se cierra el socket.
     */
    void close();


private:
    int socketDescriptor; // Descriptor del socket

    /**
     * (Solo para socket servidor)
     * Establece las opciones del socket para permitir que otro programa
     * reutilice el puerto sin tener que esperar el tiempo de espera (TIME_WAIT
     * en el caso de TCP).
     */
    void setsockopt();

    /**
     * (Solo para socket servidor)
     * Asocia el socket a un puerto (para decirle al SO que deseamos atender
     * a un determinado servicio, de forma que cuando llegue un mensaje por ese
     * servicio nos avise).
     */
    void bind();

    /**
     * (Solo para socket servidor)
     * Habilita el socket para que pueda recibir conexiones.
     */
    void listen();

    /**
     * (Solo para socket cliente)
     * Solicita la conexión con el servidor.
     */
    void connect(struct sockaddr_in &serverAddress);
};


#endif //DOMINOES_OVER_TCP_SOCKET_H
