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
#include <vector>
#include <sstream>


using std::ifstream;
using std::ofstream;
using std::endl;
using std::cerr;
using std::vector;


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

bool UsersManager::usersFileExists() const {
    ifstream f(usersFilePath);
    return f.good();
}

bool UsersManager::usernameExists(const string &username) const {
    if (!usersFileExists())
        return false;

    ifstream inFile(usersFilePath);
    if (!inFile) {
        cerr << "Error al leer del fichero " << usersFilePath << endl;
        exit(EXIT_FAILURE);
    }

    string line, readUsername;
    while (inFile >> line) {
        // Separamos username de la contraseña
        std::stringstream check1(line);
        getline(check1, readUsername, ',');

        if (username == readUsername) {
            inFile.close();
            return true;
        }
    }

    inFile.close();
    return false;
}

int UsersManager::getNRegisteredUsers() const {
    if (!usersFileExists())
        return 0;

    ifstream inFile(usersFilePath);
    if (!inFile) {
        cerr << "Error al leer del fichero " << usersFilePath << endl;
        exit(EXIT_FAILURE);
    }

    string unused;
    int nLines = 0;
    while (inFile >> unused)
        ++nLines;

    inFile.close();
    return nLines;
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
        cerr << "Error al escribir en el fichero " << usersFilePath << endl;
        exit(EXIT_FAILURE);
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

    if (usernameExists(username)) {
        User *user = getUserPtr(userSocketD);
        user->setUsername(username);
        user->setUsernameLogged(true);

        return true;
    }

    return false;
}

bool UsersManager::loginPassword(int userSocketD, const string &password) {
    if (!usersFileExists())
        return false;

    User *user = getUserPtr(userSocketD);

    ifstream inFile(usersFilePath);
    if (!inFile) {
        cerr << "Error al leer del fichero " << usersFilePath << endl;
        exit(EXIT_FAILURE);
    }

    string line, readUsername, readPassword;
    while (inFile >> line) {
        // Separamos username y contraseña
        std::stringstream check1(line);
        getline(check1, readUsername, ',');
        getline(check1, readPassword, ',');

        if (user->getUsername() == readUsername && password == readPassword) {
            user->setPasswordLogged(true);
            inFile.close();
            return true;
        }
    }

    inFile.close();
    return false;
}
