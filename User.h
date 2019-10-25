/**
 * @file    User.h
 * @author  Andrés Salinas Lima
 * @date    18/10/2019
 * @brief   Declaración de la clase User.
 */

#ifndef DOMINOES_OVER_TCP_USER_H
#define DOMINOES_OVER_TCP_USER_H


class DominoesBoard; // Para evitar error con clases que se referencian entre sí

#include "DominoesBoard.h"
#include "DominoTile.h"

#include <string>
#include <list>


using namespace std;


/**
 * @class User: Representa a los usuarios conectados.
 */
class User {
public:
    /**
     * Constructor: Crea un nuevo usuario con el descriptor de socket pasado
     * como argumento y con todos los booleanos inicializados a false.
     */
    inline explicit User(int socketDescriptor) : socketDescriptor(socketDescriptor) {}

    /**
     * Devuelve el descriptor del socket en el que se conectó el usuario.
     */
    inline int getSocketDescriptor() const {
        return socketDescriptor;
    }

    /**
     * Devuelve el username del usuario.
     */
    inline const string &getUsername() const {
        return username;
    }

    /**
     * Establece el nombre del usuario.
     */
    inline void setUsername(const string &username) {
        User::username = username;
    }

    /**
     * Indica si el usuario ha introducido su username correctamente.
     */
    inline bool isUsernameLogged() const {
        return usernameLogged;
    }

    /**
     * Establece el estado de usernameLogged.
     */
    inline void setUsernameLogged(bool usernameLogged) {
        User::usernameLogged = usernameLogged;
    }

    /**
     * Indica si el usuario ha introducido su contraseña correctamente.
     */
    inline bool isPasswordLogged() const {
        return passwordLogged;
    }

    /**
     * Establece el estado de passwordLogged.
     */
    inline void setPasswordLogged(bool passwordLogged) {
        User::passwordLogged = passwordLogged;
    }

    /**
     * Indica si el usuario está buscando partida.
     */
    inline bool isWaiting() const {
        return waiting;
    }

    /**
     * Establece el estado de waiting.
     */
    inline void setWaiting(bool waiting) {
        User::waiting = waiting;
    }

    /**
     * Indica si el usuario está jugando.
     */
    inline bool isPlaying() const {
        return playing;
    }

    /**
     * Establece el estado de playing.
     */
    inline void setPlaying(bool playing) {
        User::playing = playing;
    }

    /**
     * Devuelve una referencia a la lista de fichas del usuario.
     */
    inline list <DominoTile> &getDominoTiles() {
        return dominoTiles;
    }

    /**
     * Devuelve el puntero al tablero de Dominó en el que el usuario está jugando.
     */
    inline DominoesBoard *getDominoesBoard() const {
        return dominoesBoard;
    }

    /**
     * Establece el puntero al tablero de Dominó en el que el usuario está jugando.
     */
    inline void setDominoesBoard(DominoesBoard *dominoesBoard) {
        User::dominoesBoard = dominoesBoard;
    }

    /**
     * Devuelve el puntero al usuario contra el que este usuario está jugando.
     */
    inline User *getOpponent() const {
        return opponent;
    }

    /**
     * Establece el puntero al usuario contra el que este usuario está jugando.
     */
    inline void setOpponent(User *opponent) {
        User::opponent = opponent;
    }

    /**
     * Indica si es el turno de este usuario.
     */
    inline bool isMyTurn() const {
        return myTurn;
    }

    /**
     * Establece si es el turno de este usuario.
     */
    inline void setMyTurn(bool myTurn) {
        User::myTurn = myTurn;
    }

private:
    int socketDescriptor;        // Descriptor del socket en el que se conectó
    // el usuario.
    string username = "";        // Nombre de usuario.
    bool usernameLogged = false; // Indica si el usuario ha introducido su
    // username correctamente
    bool passwordLogged = false; // Indica si el usuario ha introducido su
    // contraseña correctamente
    bool waiting = false;        // Indica si el usuario está buscando partida
    bool playing = false;        // Indica si el usuario está jugando
    bool myTurn = false;         // Indica si es el turno del usuario
    list <DominoTile> dominoTiles;          // Fichas de dominó del jugador
    DominoesBoard *dominoesBoard = nullptr; // Puntero al tablero de dominó en el
    // que el usuario está jugando
    User *opponent = nullptr;               // Puntero al jugador oponente
};


#endif //DOMINOES_OVER_TCP_USER_H
