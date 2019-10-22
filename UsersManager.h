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
#include "constants.h"

#include <list>


using std::list;


/**
 * Clase UsersManager: Gestiona la lista de usuarios conectados y les permite
 * registrarse e iniciar sesión.
 */
class UsersManager {
// TODO: Documentar miembros clase
public:
    void addUser(int userSocketD);

    void removeUser(int userSocketD);

    /**
     * Devuelve el usuario con el descriptor de socket especificado.
     * @warning Si no existiera ningún usuario con ese descriptor de socket se
     * devuelve un new User(-1)
     */
    const User &getUser(int userSocketD) const;

    inline const list<User> &getUsers() const {
        return users;
    }

    inline int getNUsers() const {
        return users.size();
    }

    int getNRegisteredUsers() const;

    bool registerUser(int userSocketD, const string &username, const string &password);

    bool loginUsername(int userSocketD, const string &username);

    bool loginPassword(int userSocketD, const string &password);


private:
    list<User> users; // Lista de usuarios conectados
    const string usersFilePath = USERS_FILE_PATH; // Path del fichero donde se
    // almacenan los usuarios y contraseñas en TEXTO PLANO (separado por comas).

    User *getUserPtr(int userSocketD);

    bool usersFileExists();

    bool usernameExists(const string &username);

};


#endif //DOMINOES_OVER_TCP_USERSMANAGER_H
