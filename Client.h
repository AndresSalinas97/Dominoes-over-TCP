/**
 * @file    Client.h
 * @author  Andrés Salinas Lima
 * @date    18/10/2019
 * @brief   Declaración de la clase Client: TODO
 */

#ifndef DOMINOES_OVER_TCP_CLIENT_H
#define DOMINOES_OVER_TCP_CLIENT_H


#include <string>


using std::string;


class Client {
public:
    explicit Client(int socketDescriptor);

    inline int getSocketDescriptor() const {
        return socketDescriptor;
    }

    inline const string &getUsername() const {
        return username;
    }

    inline bool isUsernameLogged() const {
        return usernameLogged;
    }

    inline bool isPasswordLogged() const {
        return passwordLogged;
    }

    inline bool isWaiting() const {
        return waiting;
    }


private:
    int socketDescriptor;
    string username;
    bool usernameLogged;
    bool passwordLogged;
    bool waiting;
    //TODO: Pointer to domino board
};


#endif //DOMINOES_OVER_TCP_CLIENT_H
