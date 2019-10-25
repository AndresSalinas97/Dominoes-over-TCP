/**
 * @file    DominoesBoard.cpp
 * @author  Andrés Salinas Lima
 * @date    24/10/2019
 * @brief   Código de la clase DominoesBoard.
 */


#include "DominoesBoard.h"

#include <vector>
#include <algorithm>
#include <random>


void DominoesBoard::shuffle(list<DominoTile> &player1Tiles,
                            list<DominoTile> &player2Tiles) {
    vector<DominoTile> temp;

    // Generamos las 28 fichas y las añadimos al vector temp
    for (int i = 0; i <= 6; i++)
        for (int j = i; j <= 6; j++)
            temp.push_back(*new DominoTile(i, j));

    // Barajamos las fichas
    unsigned seed = time(nullptr);
    ::shuffle(temp.begin(), temp.end(), default_random_engine(seed));

    // Las primeras 7 fichas son para el jugador 1
    for (int i = 0; i < 7; i++)
        player1Tiles.push_back(temp[i]);

    // Las siguientes 7 fichas son para el jugador 2
    for (int i = 7; i < 14; i++)
        player2Tiles.push_back(temp[i]);

    // El resto de fichas se quedan en la mesa
    for (int i = 14; i < 28; i++)
        sleepingTiles.push_back(temp[i]);
}


int DominoesBoard::getDominoTileScore(const DominoTile &dominoTile) {
    if (dominoTile.getLeft() == dominoTile.getRight())
        return dominoTile.getLeft() + dominoTile.getRight() + 100;
    else
        return dominoTile.getLeft() + dominoTile.getRight();
}

const DominoTile &DominoesBoard::getBestDominoTile(const list<DominoTile> &dominoes) {
    const DominoTile *bestTile = nullptr;
    int bestScore = 0;
    int score;

    for (auto &domino : dominoes) {
        score = getDominoTileScore(domino);
        if (score > bestScore) {
            bestScore = score;
            bestTile = &domino;
        }
    }

    return *bestTile;
}

User *DominoesBoard::whoStarts(User *player1, User *player2) {
    DominoTile bestPlayer1Tile = getBestDominoTile(player1->getDominoTiles());
    DominoTile bestPlayer2Tile = getBestDominoTile(player2->getDominoTiles());

    if (getDominoTileScore(bestPlayer1Tile) > getDominoTileScore(bestPlayer2Tile))
        return player1;
    else
        return player2;
}
