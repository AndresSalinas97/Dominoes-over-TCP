/**
 * @file    mainServer.cpp
 * @author  Andrés Salinas Lima
 * @date    07/10/2019
 * @brief   Servidor TCP para el juego del Domino: Gestiona clientes, usuarios,
 *          partidas y la logica del juego
 */


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define SERVER_PORT 2050 // Puerto en el que el servidor acepta el servicio
#define MSG_SIZE 250     // Longitud máxima de los mensajes entre cliente y servidor
#define MAX_CLIENTS 5    // Número máximo clientes conectados al servidor
#define MAX_DOMINOES 2   // Número máximo de partidas simultáneas


using std::cout;
using std::cerr;
using std::cin;
using std::endl;


// TODO: Capturar señales (ctrl+c)
int main() {
    // Se abre el socket del servidor:
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        cerr << "No se puede abrir el socket: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }


    // Para permitir que otro programa reutilice el puerto sin tener que esperar
    // el tiempo de espera (TIME_WAIT en el caso de TCP):
    int on = 1;
    if ((setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0) {
        cerr << "Error en setsockopt(): " << strerror(errno) << endl;
        close(sd);
        exit(EXIT_FAILURE);
    }


    // Se rellenan los campos de la estructura de la dirección del servidor
    // (necesario para la llamada a la funcion bind()):
    struct sockaddr_in server_address = {};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // Permite recibir
    // paquetes destinados a cualquiera de las interfaces:


    // Asociamos el socket a un puerto (para decirle al SO que deseamos atender
    // a un determinado servicio, de forma que cuando llegue un mensaje por ese
    // servicio nos avise):
    if (bind(sd, (struct sockaddr *) &server_address,
             sizeof(server_address)) < 0) {
        cerr << "Error en bind(): " << strerror(errno) << endl;
        close(sd);
        exit(EXIT_FAILURE);
    }


    // Habilitamos el socket para que pueda recibir conexiones:
    if (listen(sd, 1) < 0) {
        cerr << "Error en listen(): " << strerror(errno) << endl;
        close(sd);
        exit(EXIT_FAILURE);
    }


    // Inicializamos los conjuntos fd_set para la función select():
    fd_set read_fds, aux_fds; // Sets de descriptores para la funcion select()
    FD_ZERO(&aux_fds); // Vacía el el set
    FD_ZERO(&read_fds); // Vacía el set
    FD_SET((unsigned) 0, &read_fds); // Añade stdin al set de descriptores de lectura
    FD_SET(sd, &read_fds); // Añade el socket al set de descriptores de lectura


    // Comunicación con los clientes:
    char message_received[MSG_SIZE], message_sent[MSG_SIZE];
    bool end = false;
    int new_client_sd;
    struct sockaddr_in client_address = {};
    socklen_t client_address_length = sizeof(client_address);
    do {
        aux_fds = read_fds; // Inicializamos aux_fds antes de llamar a select()

        // select() duerme el proceso hasta que haya datos disponibles en alguno de los sockets del set
        if ((select(sd + 1, &aux_fds, nullptr, nullptr, nullptr)) < 0) {
            cerr << "Error en select: " << strerror(errno) << endl;
            end = true;
        }

        for (int i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &aux_fds)) {

                if (i == 0) {
                    // El usuario ha tecleado un mensaje
                    // TODO: Procesar el mensaje que se ha tecleado
                } else if (i == sd) {
                    // Tenemos nuevos datos en el socket del servidor

                    // Aceptamos la conexión obteniendo el socket del nuevo cliente
                    if ((new_client_sd = accept(sd, (struct sockaddr *) &client_address,
                                                    &client_address_length)) < 0) {
                        cerr << "Error aceptando petición: " << strerror(errno) << endl;
                    } else {
                        // Tenemos un nuevo cliente
                        // TODO: Comprobar que no superemos el limite de clientes
                        FD_SET(new_client_sd, &read_fds);
                        cout << "Tenemos un nuevo cliente: " << new_client_sd << end;
                        bzero(message_sent, sizeof(message_sent));
                        strcpy(message_sent, "Bienvenido");
                        if ((send(new_client_sd, message_sent, strlen(message_sent), 0)) < 0)
                            cerr << "Error al enviar mensaje: " << strerror(errno) << endl;
                    }
                } else {
                    // Tenemos nuevos datos en el socket de algún cliente
                    bzero(message_received, sizeof(message_received));
                    if ((recv(i, message_received, sizeof(message_received), 0)) < 0) {
                        cerr << "Error al recibir mensaje: " << strerror(errno) << endl;
                    }
                    cout << "Cliente " << i << ": " << message_received << endl;
                }
            }
        }
    } while (!end);


    close(sd);
    return EXIT_SUCCESS;
}
