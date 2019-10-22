/**
 * @file    UsersManager.cpp
 * @author  Andrés Salinas Lima
 * @date    18/10/2019
 * @brief   Código de la clase UsersManager: Gestiona la lista de usuarios
 *          conectados y les permite registrarse e iniciar sesión.
 */


#include "UsersManager.h"
#include <fstream>
#include <iostream>


using std::ifstream;
using std::ofstream;
using std::endl;
using std::cerr;


void UsersManager::addUser(int userSocketD) {
    User user(userSocketD);
    users.push_back(user);
}

void UsersManager::removeUser(int userSocketD) {
    for (auto user = users.begin(); user != users.end(); user++)
        if (user->getSocketDescriptor() == userSocketD) {
            users.erase(user);
            break;
        }
}

const User &UsersManager::getUser(int userSocketD) const {
    for (auto &user : users)
        if (user.getSocketDescriptor() == userSocketD)
            return user;

    return *new User(-1);
}

User *UsersManager::getUserPtr(int userSocketD) {
    for (auto &user : users)
        if (user.getSocketDescriptor() == userSocketD)
            return &user;

    return nullptr;
}

bool UsersManager::usersFileExists() {
    ifstream f(usersFilePath);
    return f.good();
}

bool UsersManager::usernameExists(const string &username) {
    // TODO
    return false;
}

int UsersManager::getNRegisteredUsers() const {
    // TODO
    return 0;
}

bool UsersManager::registerUser(int userSocketD, const string &username,
                                const string &password) {
    // Comprobamos que el username no exista ya
    if (usernameExists(username))
        return false;

    // Grabamos el username y contraseña en el fichero
    ofstream outFile(usersFilePath, std::ios_base::app); // Modo append: si no
    // existe el fichero se crea, si existe se escribe al final.
    outFile << username << "," << password << endl;
    outFile.close();

    if (!outFile) {
        cerr << "Error al escribir en fichero " << usersFilePath << endl;
        return false;
    }

    // Hacemos login al usuario
    User *user = getUserPtr(userSocketD);
    user->setUsername(username);
    user->setUsernameLogged(true);
    user->setPasswordLogged(true);

    return true;
}

bool UsersManager::loginUsername(int userSocketD, const string &username) {
    if (!usersFileExists())
        return false;

    return usernameExists(username);
}

bool UsersManager::loginPassword(int userSocketD, const string &password) {
    if (!usersFileExists())
        return false;

    // TODO
    return false;
}
