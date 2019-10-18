/**
 * @file    UsersManager.cpp
 * @author  Andrés Salinas Lima
 * @date    18/10/2019
 * @brief   Código de la clase UsersManager: TODO
 */


#include "UsersManager.h"


void UsersManager::addUser(int socketD) {
    User user(socketD);
    users.push_back(user);
}

void UsersManager::removeUser(int socketD) {
    for (auto user = users.begin(); user != users.end(); user++)
        if (user->getSocketDescriptor() == socketD) {
            users.erase(user);
            break;
        }
}

User *UsersManager::getUser(int socketD) {
    for (auto & user : users)
        if (user.getSocketDescriptor() == socketD)
            return &user;

    return nullptr;
}
