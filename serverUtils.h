/**
 * @file    serverUtils.h
 * @author  Andrés Salinas Lima
 * @date    11/10/2019
 * @brief   Declaración de funciones auxiliares del servidor.
 */


#ifndef DOMINOES_OVER_TCP_SERVERUTILS_H
#define DOMINOES_OVER_TCP_SERVERUTILS_H


#include <sys/socket.h>


int prepareServerSocket();
void handleUserInput();
void handleNewClient(int my_socket, fd_set &read_fds);
void handleGoneClient(int client_socket, fd_set &read_fds);
void handleClientCommunication(int active_socket, char* received_message);


#endif //DOMINOES_OVER_TCP_SERVERUTILS_H
