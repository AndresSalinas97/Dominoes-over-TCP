/**
 * @file    User.h
 * @author  Andrés Salinas Lima
 * @date    18/10/2019
 * @brief   Declaración de la clase User: TODO
 */

#ifndef DOMINOES_OVER_TCP_USER_H
#define DOMINOES_OVER_TCP_USER_H


#include <string>


using std::string;


/**
 * TODO: Documentar clase
 */
class User {
// TODO: Documentar miembros clase
public:
    inline explicit User(int socketDescriptor) :
            username(""),
            usernameLogged(false),
            passwordLogged(false),
            waiting(false),
            playing(false) {
        this->socketDescriptor = socketDescriptor;
    }

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

    inline bool isPlaying() const {
        return playing;
    }


private:
    int socketDescriptor;
    string username;
    bool usernameLogged;
    bool passwordLogged;
    bool waiting;
    bool playing;
    // TODO: Pointer to Dominoes game
};


#endif //DOMINOES_OVER_TCP_USER_H
