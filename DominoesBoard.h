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
     * Es un método para saber que ficha es mejor.
     * Esta "puntuación" será valorIzquierda + valorDerecha para fichas normales
     * y valorIzquierda + valorDerecha + 100 para fichas dobles.
     */
    static int getDominoTileScore(const DominoTile &dominoTile);
};


#endif //DOMINOES_OVER_TCP_DOMINOESBOARD_H
