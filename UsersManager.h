/**
 * @file    UsersManager.h
 * @author  Andrés Salinas Lima
 * @date    18/10/2019
 * @brief   Declaración de la clase UsersManager: Gestiona la lista de usuarios
 *          conectados y les permite registrarse e iniciar sesión.
 */


#ifndef DOMINOES_OVER_TCP_USERSMANAGER_H
#define DOMINOES_OVER_TCP_USERSMANAGER_H


#include "User.h"

#include <list>


using std::list;


/**
 * Clase UsersManager: Gestiona la lista de usuarios conectados y les permite
 * registrarse e iniciar sesión.
 */
class UsersManager {
// TODO: Documentar miembros clase
public:
    void addUser(int socketD);

    void removeUser(int socketD);

    /**
     * Devuelve el usuario con el descriptor de socket especificado.
     * @warning Si no existiera ningún usuario con ese descriptor de socket se
     * devuelve un new User(-1)
     */
    const User &getUser(int socketD) const;

    inline const list<User> &getUsers() const {
        return users;
    }

    inline int getNUsers() const {
        return users.size();
    }

    // TODO: int getNRegisteredUsers() const;


private:
    list<User> users; // Lista de usuarios conectados
};


#endif //DOMINOES_OVER_TCP_USERSMANAGER_H
