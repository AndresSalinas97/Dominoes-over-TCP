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
 * @class DominoesBoard: Tablero del juego del dominó.
 */
class DominoesBoard {
// TODO: Documentar miembros clase
public:
    void shuffle(list<DominoTile> &player1Tiles, list<DominoTile> &player2Tiles);

    static const DominoTile &getBestDominoTile(const list<DominoTile> &dominoes);

    static User *whoStarts(User *player1, User *player2);

private:
    list<DominoTile> sleepingTiles;
    list<DominoTile> tableTiles;

    /**
     * Devuelve la "puntuación" de la ficha de dominó introducida.
     * Es mi método para saber que ficha es mejor.
     * Dará como resultado: |6|6| > |5|5| > ... > |0|0| > |6|5| > |6|4| > ...
     * > |6|0| > |5|4| > ...
     */
    static int getDominoTileScore(const DominoTile &dominoTile);
};


#endif //DOMINOES_OVER_TCP_DOMINOESBOARD_H
