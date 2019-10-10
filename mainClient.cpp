/**
 * @file    mainClient.cpp
 * @author  Andres Salinas Lima
 * @date    07/10/2019
 * @brief   Cliente para servidor TCP: Envia y recibe cadenas de texto
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
#define MSG_SIZE 250     // Longitud maxima de los mensajes entre cliente y servidor


using std::cout;
using std::cerr;
using std::cin;
using std::endl;


int main(int argc, char **argv) {
    // Se compruebban y procesan los argumentos pasados en la línea de comandos:
    if (argc != 2) {
        cerr << "Uso del programa: " << argv[0] << " direccionIPservidor" << endl;
        exit(EXIT_FAILURE);
    }
    char *server_IP_address = argv[1];


    // Se rellenan los campos de la estructura de la direccion del servidor:
    struct sockaddr_in server_address = {};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = inet_addr(server_IP_address);
    socklen_t server_address_length = sizeof(server_address);


    // Se abre el socket cliente:
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        cerr << "No se puede abrir el socket cliente: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }


    // Se solicita la conexion con el servidor:
    if (connect(client_socket, (struct sockaddr *) &server_address, server_address_length) < 0) {
        cerr << "Error al conectar con el servidor: " << strerror(errno) << endl;
        close(client_socket);
        exit(EXIT_FAILURE);
    }


    // Intercambio de mensajes con el servidor:
    char message_received[MSG_SIZE], message_sent[MSG_SIZE];
    bool end = false;
    fd_set read_fds, aux_fds; // Sets de descriptores para la funcion select()
    FD_ZERO(&aux_fds); // Vacia el puntero
    FD_ZERO(&read_fds); // Vacia el puntero
    FD_SET((unsigned) STDIN_FILENO, &read_fds); // Anade stdin al set de descriptores de lectura
    FD_SET(client_socket, &read_fds); // Anade el socket al set de descriptores de lectura

    do {
        aux_fds = read_fds; // Inicializamos aux_fds antes de llamar a select()

        if ((select(client_socket + 1, &aux_fds, nullptr, nullptr, nullptr)) < 0) {
            cerr << "Error en select: " << strerror(errno) << endl;
            end = true;
        }

        if (FD_ISSET(client_socket, &aux_fds)) {
            // Hemos recibido un mensaje del servidor

            bzero(message_received, sizeof(message_received));

            if ((recv(client_socket, message_received, sizeof(message_received), 0)) < 0) {
                cerr << "Error al recibir mensaje del servidor: " << strerror(errno) << endl;
                end = true;
            }

            cout << message_received << endl;

            // TODO: Desconectar cliente si el servidor nos rechaza
            if (strcmp(message_received, "") == 0)
                end = true;

        } else if (FD_ISSET(STDIN_FILENO, &aux_fds)) {
            // El usuario ha tecleado un mensaje

            bzero(message_sent, sizeof(message_sent));

            cin.get(message_sent, MSG_SIZE);
            cin.ignore();

            if ((send(client_socket, message_sent, sizeof(message_sent), 0)) < 0) {
                cerr << "Error al enviar mensaje al servidor: " << strerror(errno) << endl;
                end = true;
            }

            if (strcmp(message_sent, "SALIR") == 0)
                end = true;
        }
    } while (!end);

    return 0;
}
