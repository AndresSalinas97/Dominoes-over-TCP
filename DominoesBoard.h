/**
 * @file    DominoesBoard.h
 * @author  Andrés Salinas Lima
 * @date    24/10/2019
 * @brief   Declaración de la clase DominoesBoard.
 */


#ifndef DOMINOES_OVER_TCP_DOMINOESBOARD_H
#define DOMINOES_OVER_TCP_DOMINOESBOARD_H


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

private:
    list<DominoTile> sleepingTiles;
    list<DominoTile> tableTiles;
};


#endif //DOMINOES_OVER_TCP_DOMINOESBOARD_H
