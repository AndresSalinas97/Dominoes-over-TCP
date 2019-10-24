/**
 * @file    Domino.cpp
 * @author  Andrés Salinas Lima
 * @date    24/10/2019
 * @brief   Código de la clase Domino.
 */


#include "Domino.h"


void Domino::flip() {
    unsigned short temp = right;
    right = left;
    left = right;
}

ostream &operator<<(ostream &os, const Domino &d) {
    os << "|" << d.getLeft() << "|" << d.getRight() << "|";
    return os;
}

istream &operator>>(istream &stream, Domino &d) {
    string temp;

    getline(stream, temp, '|');

    getline(stream, temp, '|');
    d.setLeft(strtol(temp.c_str(), nullptr, 10));

    getline(stream, temp, '|');
    d.setRight(strtol(temp.c_str(), nullptr, 10));

    return stream;
}
