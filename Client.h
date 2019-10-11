/**
 * @file    Client.h
 * @author  Andrés Salinas Lima
 * @date    11/10/2019
 * @brief   Clase cliente: TODO
 */

#ifndef DOMINOES_OVER_TCP_CLIENT_H
#define DOMINOES_OVER_TCP_CLIENT_H


#include <string>


using std::string;


class Client {
public:


private:
    int socket;
    bool logged;
    string username;
    bool playing;
    //TODO: Pointer to domino board


public:
    inline Client(int socket) : logged(false), username("") {
        this->socket = socket;
    }

    int getSocket() const {
        return socket;
    }

    bool isLogged() const {
        return logged;
    }

    void setSocket(int socket) {
        Client::socket = socket;
    }

    const string &getUsername() const {
        return username;
    }

    void setUsername(const string &username) {
        Client::username = username;
    }
};


#endif //DOMINOES_OVER_TCP_CLIENT_H
