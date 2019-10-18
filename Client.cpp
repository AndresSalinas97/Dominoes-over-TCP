/**
 * @file    Client.h
 * @author  Andrés Salinas Lima
 * @date    18/10/2019
 * @brief   Código de la clase Client: TODO
 */


#include "Client.h"


Client::Client(int socketDescriptor) :
        username(""),
        usernameLogged(false),
        passwordLogged(false),
        waiting(false) {
    this->socketDescriptor = socketDescriptor;
}
