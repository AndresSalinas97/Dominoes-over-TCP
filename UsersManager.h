/**
 * @file    UsersManager.h
 * @author  Andrés Salinas Lima
 * @date    18/10/2019
 * @brief   Declaración de la clase UsersManager: TODO
 */


#ifndef DOMINOES_OVER_TCP_USERSMANAGER_H
#define DOMINOES_OVER_TCP_USERSMANAGER_H


#include "User.h"

#include <list>


using std::list;


/**
 * TODO: Documentar clase
 */
class UsersManager {
// TODO: Documentar miembros clase
public:
    void addUser(int socketD);

    void removeUser(int socketD);

    User *getUser(int socketD);

    inline const list<User> &getUsers() const { return users; }

    inline int getNUsers() const { return users.size(); }

    //int getNRegisteredUsers() const;


private:
    list<User> users; // Lista de usuarios conectados

};


#endif //DOMINOES_OVER_TCP_USERSMANAGER_H
