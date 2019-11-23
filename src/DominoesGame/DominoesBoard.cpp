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


void DominoesBoard::shuffle(list<DominoTile> &player1Tiles, list<DominoTile> &player2Tiles) {
    vector<DominoTile> temp;

    // Generamos las 28 fichas y las añadimos al vector temp
    for (int i = 6; i >= 0; i--)
        for (int j = i; j >= 0; j--)
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
    if (dominoTile.getLeft() == dominoTile.getRight()) {
        return dominoTile.getLeft() + 1000;
    } else {
        if (dominoTile.getLeft() > dominoTile.getRight())
            return dominoTile.getLeft() * 100 + dominoTile.getRight();
        else
            return dominoTile.getLeft() + dominoTile.getRight() * 100;
    }

    /**
     * De esta manera las fichas dobles siempre van a tener mejor puntuación.
     * De esta manera fichas como |6|0| (puntuación = 600) tendrán mejor que,
     * por ejemplo |4|5| (puntuación = 504).
     */
}

const DominoTile &DominoesBoard::getBestDominoTile(const list<DominoTile> &dominoes) {
    const DominoTile *bestTile = new DominoTile(-1, -1);
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

bool DominoesBoard::canPlayerPlay(User &user) {
    if (boardIsEmpty())
        return true;

    for (auto &dominoTile : user.getDominoTiles())
        if (dominoTile.has(getLeftValue()) || dominoTile.has(getRightValue()))
            return true;

    return false;
}

unsigned short DominoesBoard::getLeftValue() const {
    if (boardIsEmpty())
        return -1;

    return boardTiles.begin()->getLeft();
}

unsigned short DominoesBoard::getRightValue() const {
    if (boardIsEmpty())
        return -1;

    return boardTiles.back().getRight();
}

bool DominoesBoard::placeTile(const DominoTile &dominoTile) {
    if (!boardIsEmpty())
        return false;

    boardTiles.push_front(dominoTile);

    return true;
}

bool DominoesBoard::placeTileLeft(const DominoTile &dominoTile) {
    if (boardIsEmpty())
        return false;

    if (dominoTile.getRight() == getLeftValue()) {
        boardTiles.push_front(dominoTile);
        return true;
    }

    // Le tenemos que dar la vuelta a la ficha
    if (dominoTile.getLeft() == getLeftValue()) {
        DominoTile temp = dominoTile;
        temp.flip();
        boardTiles.push_front(temp);
        return true;
    }

    return false;
}

bool DominoesBoard::placeTileRight(const DominoTile &dominoTile) {
    if (boardIsEmpty())
        return false;

    if (dominoTile.getLeft() == getRightValue()) {
        boardTiles.push_back(dominoTile);
        return true;
    }

    // Le tenemos que dar la vuelta a la ficha
    if (dominoTile.getRight() == getRightValue()) {
        DominoTile temp = dominoTile;
        temp.flip();
        boardTiles.push_back(temp);
        return true;
    }

    return false;
}

bool DominoesBoard::checkWinner(User *player1, User *player2, User **winner) {
    // El jugador 1 ha colocado todas sus fichas
    if (player1->getDominoTiles().empty()) {
        *winner = player1;
        return true;
    }

    // El jugador 2 ha colocado todas sus fichas
    if (player2->getDominoTiles().empty()) {
        *winner = player2;
        return true;
    }

    // Partida cerrada: Ningún jugador puede continuar la partida y no quedan
    // fichas para robar
    if (!canPlayerPlay(*player1) && !canPlayerPlay(*player2) && sleepingTilesIsEmpty()) {
        if (player1->getDominoTiles().size() < player2->getDominoTiles().size())
            *winner = player1;
        else if (player1->getDominoTiles().size() > player2->getDominoTiles().size())
            *winner = player2;
        else
            *winner = nullptr; // Empate
        return true;
    }

    return false;
}

const DominoTile &DominoesBoard::takeSleepingTile() {
    auto *d = new DominoTile(sleepingTiles.back().getLeft(),
                             sleepingTiles.back().getRight());
    sleepingTiles.pop_back();
    return *d;
}
