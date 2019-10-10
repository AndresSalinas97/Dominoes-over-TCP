/**
 * @file    mainClient.cpp
 * @author  Andrés Salinas Lima
 * @date    07/10/2019
 * @brief   Cliente para servidor TCP: Envía y recibe cadenas de texto
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


using std::cout;
using std::cerr;
using std::cin;
using std::endl;


// TODO: Capturar señales (ctrl+c)
int main(int argc, char **argv) {
    // Se compruebban y procesan los argumentos pasados por la línea de comandos:
    if (argc != 2) {
        cerr << "Uso del programa: " << argv[0] << " direcciónIPservidor" << endl;
        exit(EXIT_FAILURE);
    }
    char *server_IP_address = argv[1];


    // Se rellenan los campos de la estructura de la dirección del servidor:
    struct sockaddr_in server_address = {};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(server_IP_address);
    socklen_t server_address_length = sizeof(server_address);


    // Se abre el socket del cliente:
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        cerr << "No se puede abrir el socket: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }


    // Se solicita la conexión con el servidor:
    if (connect(sd, (struct sockaddr *) &server_address, server_address_length) < 0) {
        cerr << "Error al conectar con el servidor: " << strerror(errno) << endl;
        close(sd);
        exit(EXIT_FAILURE);
    }


    // Inicializamos los conjuntos fd_set para la función select()
    fd_set read_fds, aux_fds; // Sets de descriptores para la funcion select()
    FD_ZERO(&aux_fds); // Vacía el el set
    FD_ZERO(&read_fds); // Vacía el set
    FD_SET((unsigned) 0, &read_fds); // Añade stdin al set de descriptores de lectura
    FD_SET(sd, &read_fds); // Añade el socket al set de descriptores de lectura


    // Intercambio de mensajes con el servidor:
    char message_received[MSG_SIZE], message_sent[MSG_SIZE];
    bool end = false;
    do {
        aux_fds = read_fds; // Inicializamos aux_fds antes de llamar a select()

        // select() duerme el proceso hasta que haya datos disponibles en alguno de los sockets del set
        if ((select(sd + 1, &aux_fds, nullptr, nullptr, nullptr)) < 0) {
            cerr << "Error en select: " << strerror(errno) << endl;
            end = true;
        }

        if (FD_ISSET(sd, &aux_fds)) {
            // Hemos recibido un mensaje del servidor en el socket

            bzero(message_received, sizeof(message_received));

            if ((recv(sd, message_received, sizeof(message_received), 0)) < 0) {
                cerr << "Error al recibir mensaje del servidor: " << strerror(errno) << endl;
                end = true;
            }

            cout << message_received << endl;

            // TODO: Desconectar cliente si el servidor nos rechaza o algo
            if (strcmp(message_received, "") == 0)
                end = true;

        } else if (FD_ISSET(0, &aux_fds)) {
            // El usuario ha tecleado un mensaje

            bzero(message_sent, sizeof(message_sent));

            cin.get(message_sent, MSG_SIZE);
            cin.ignore();

            if ((send(sd, message_sent, sizeof(message_sent), 0)) < 0) {
                cerr << "Error al enviar mensaje al servidor: " << strerror(errno) << endl;
                end = true;
            }

            if (strcmp(message_sent, "SALIR") == 0)
                end = true;
        }
    } while (!end);

    return EXIT_SUCCESS;
}
