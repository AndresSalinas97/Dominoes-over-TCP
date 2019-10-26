/**
 * @file    DominoTile.cpp
 * @author  Andrés Salinas Lima
 * @date    24/10/2019
 * @brief   Código de la clase DominoTile.
 */


#include "DominoTile.h"


void DominoTile::flip() {
    unsigned short temp = right;
    right = left;
    left = right;
}

bool DominoTile::operator==(const DominoTile &dominoTile) {
    return (left == dominoTile.left && right == dominoTile.right);
}

ostream &operator<<(ostream &os, const DominoTile &d) {
    os << "|" << d.getLeft() << "|" << d.getRight() << "|";
    return os;
}

istream &operator>>(istream &stream, DominoTile &d) {
    string temp;

    getline(stream, temp, '|');

    getline(stream, temp, '|');
    d.setLeft(strtol(temp.c_str(), nullptr, 10));

    getline(stream, temp, '|');
    d.setRight(strtol(temp.c_str(), nullptr, 10));

    return stream;
}
