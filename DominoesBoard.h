/**
 * @file    DominoesBoard.h
 * @author  Andrés Salinas Lima
 * @date    24/10/2019
 * @brief   Declaración de la clase DominoesBoard.
 */


#ifndef DOMINOES_OVER_TCP_DOMINOESBOARD_H
#define DOMINOES_OVER_TCP_DOMINOESBOARD_H


class User; // Para evitar error con clases que se referencian entre sí

#include "User.h"
#include "DominoTile.h"

#include <list>


using namespace std;


/**
 * @class DominoesBoard: Tablero del juego del dominó. Gestiona las fichas que
 * están sobre la mesa y contiene funciones auxiliares para facilitar la
 * gestión del juego al servidor.
 */
class DominoesBoard {
public:
    /**
     * Genera las fichas, las mezcla y las reparte entre las listas de los
     * jugadores pasadas por referencia y el montón que se queda en la mesa.
     */
    void shuffle(list<DominoTile> &player1Tiles, list<DominoTile> &player2Tiles);

    /**
     * Devuelve la mejor ficha de la lista que se pasa como referencia.
     */
    static const DominoTile &getBestDominoTile(const list<DominoTile> &dominoes);

    /**
     * Devuelve un puntero al jugador que deberá empezar la partida a partir
     * de los dos punteros a usuario que se pasan.
     */
    static User *whoStarts(User *player1, User *player2);

    /**
     * Devuelve true si no hay fichas sobre el tablero.
     */
    inline bool boardIsEmpty() const {
        return boardTiles.empty();
    }

    /**
     * Devuelve true si ya no quedan fichas en el montón.
     */
    inline bool sleepingTilesIsEmpty() const {
        return sleepingTiles.empty();
    }

    /**
     * Devuelve el valor izquierdo de la ficha que está más a la izquierda del
     * tablero.
     */
    unsigned short getLeftValue() const;

    /**
     * Devuelve el valor derecho de la ficha que está más a la derecha del
     * tablero.
     */
    unsigned short getRightValue() const;

    /**
     * Devuelve true si el jugador pasado por referencia tiene al menos una
     * ficha que podría colocar en el tablero.
     */
    bool canPlayerPlay(User &user);

    /**
     * Coloca la ficha pasada por referencia en el tablero vacío.
     * Devuelve true si la acción se realizó con éxito.
     * @warning Para usar solo en el primer movimiento, cuando el tablero está
     * vacío.
     */
    bool placeTile(const DominoTile &dominoTile);

    /**
     * Coloca la ficha pasada por referencia en el extremo izquierdo del tablero.
     * Devuelve true si la acción se realizó con éxito.
     */
    bool placeTileLeft(const DominoTile &dominoTile);

    /**
     * Coloca la ficha pasada por referencia en el derecho izquierdo del tablero.
     * Devuelve true si la acción se realizó con éxito.
     */
    bool placeTileRight(const DominoTile &dominoTile);

    /**
     * Devuelve una referencia constante a la lista de fichas sobre el tablero.
     */
    inline const list<DominoTile> &getBoardTiles() const {
        return boardTiles;
    }

    /**
     * Comprueba si hay ganador entre los jugadores pasados por referencia.
     * En caso de que haya ganador la función devolverá true y winner apuntará
     * al ganador.
     */
    bool checkWinner(User *player1, User *player2, User **winner);

    /**
     * Permite coger una ficha del montón. Devuelve una referencia a la última
     * ficha del montón y la elimina del montón.
     */
    const DominoTile &takeSleepingTile();

private:
    list<DominoTile> sleepingTiles; /** Lista de fichas del montón de fichas para robar. */
    list<DominoTile> boardTiles;    /** Lista de fichas sobre el tablero. */

    /**
     * Devuelve la "puntuación" de la ficha de dominó introducida.
     * Es mi método para saber que ficha es mejor.
     * Dará como resultado: |6|6| > |5|5| > ... > |0|0| > |6|5| > |6|4| > ...
     * > |6|0| > |5|4| > ...
     */
    static int getDominoTileScore(const DominoTile &dominoTile);
};


#endif //DOMINOES_OVER_TCP_DOMINOESBOARD_H
