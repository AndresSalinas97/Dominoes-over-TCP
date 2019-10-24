/**
 * @file    UsersManager.h
 * @author  Andrés Salinas Lima
 * @date    18/10/2019
 * @brief   Declaración de la clase UsersManager.
 */


#ifndef DOMINOES_OVER_TCP_USERSMANAGER_H
#define DOMINOES_OVER_TCP_USERSMANAGER_H


#include "User.h"
#include "constants.h"

#include <list>


using std::list;


/**
 * @class UsersManager: Gestiona la lista de usuarios conectados y les permite
 * registrarse e iniciar sesión.
 */
class UsersManager {
// TODO: Documentar miembros clase
public:
    /**
     * Crea un nuevo usuario con el socketD indicado y lo añade a la lista de
     * usuarios conectados.
     */
    void addUser(int userSocketD);

    /**
     * Elimina de la lista de usuarios al usuario con el socketD indicado.
     */
    void removeUser(int userSocketD);

    /**
     * Devuelve una referencia constante al usuario con el socketD especificado.
     * @warning Si no existiera ningún usuario con ese descriptor de socket se
     * devuelve un new User(-1).
     */
    const User &getUser(int userSocketD) const;

    /**
     * Devuelve la lista de usuarios (como const).
     */
    inline const list<User> &getUsers() const {
        return users;
    }

    /**
     * Devuelve el número de usuarios conectados.
     */
    inline int getNUsers() const {
        return users.size();
    }

    /**
     * Devuelve el número de usuarios registrados (número de líneas en el
     * fichero USERS_FILE_PATH).
     */
    int getNRegisteredUsers() const;

    /**
     * Registra a nuevo usuario con el username y contraseña introducidos e
     * inicia su sesión (se establecen los valores de los atributos username,
     * usernameLogged y passwordLogged del usuario con el socketD introducido).
     * Devuelve true si la operación se realiza con éxito.
     * Se comprueba si ya existe un usuario con ese nombre.
     */
    bool registerUser(int userSocketD, const string &username, const string &password);

    /**
     * Permite a un usuario introducir su username (se establecen los valores de
     * los atributos username y usernameLogged del usuario con el socketD
     * introducido).
     * Devuelve true si la operación se realiza con éxito.
     * Se comprueba que realmente exista un usuario con ese username.
     */
    bool loginUsername(int userSocketD, const string &username);

    /**
     * Permite a un usuario introducir su contraseña (se establece el valor del
     * atributo passwordLogged).
     * Devuelve true si la operación se realiza con éxito.
     * Se comprueba que la contraseña es válida para el username introducido
     * anteriormente.
     */
    bool loginPassword(int userSocketD, const string &password);

private:
    list<User> users; // Lista de usuarios conectados
    const string usersFilePath = USERS_FILE_PATH; // Path del fichero donde se
    // almacenan los usuarios y contraseñas en TEXTO PLANO (separado por comas).

    /**
     * Devuelve un puntero al usuario con el descriptor de socket especificado.
     * @warning Si no existiera ningún usuario con ese descriptor de socket se
     * devuelve nullptr.
     */
    User *getUserPtr(int userSocketD);

    /**
     * Devuelve true si el fichero de usuarios existe.
     */
    bool usersFileExists() const;

    /**
     * Devuelve true si existe un usuario con el username introducido.
     */
    bool usernameExists(const string &username) const;
};


#endif //DOMINOES_OVER_TCP_USERSMANAGER_H
