/**
 * @file    DominoTile.h
 * @author  Andrés Salinas Lima
 * @date    24/10/2019
 * @brief   Declaración de la clase DominoTile.
 */


#ifndef DOMINOES_OVER_TCP_DOMINOTILE_H
#define DOMINOES_OVER_TCP_DOMINOTILE_H


#include <iostream>
#include <string>
#include <sstream>


using namespace std;


/**
 * @class DominoTile: Representa una ficha del dominó.
 */
class DominoTile {
public:
    /**
     * Constructor: crea una nueva ficha de dominó a partir de los valores para
     * el lado derecho y el izquierdo introducidos.
     */
    inline DominoTile(unsigned short left, unsigned short right)
            : left(left), right(right) {}

    /**
     * Devuelve el valor del lado izquierdo de la ficha.
     */
    inline unsigned short getLeft() const {
        return left;
    }

    /**
     * Establece el valor del lado izquierdo de la ficha.
     */
    inline void setLeft(unsigned short left) {
        DominoTile::left = left;
    }

    /**
     * Devuelve el valor del lado izquierdo de la ficha.
     */
    inline unsigned short getRight() const {
        return right;
    }

    /**
     * Establece el valor del lado derecho de la ficha.
     */
    inline void setRight(unsigned short right) {
        DominoTile::right = right;
    }

    /**
     * Le da la vuelta a la ficha (intercambia left y right).
     */
    void flip();

    /**
     * Devuelve true si la ficha contiene el valor v en alguno de sus lados.
     */
    inline bool has(unsigned short v) const {
        return (left == v || right == v);
    }

    bool operator==(const DominoTile &dominoTile);

private:
    unsigned short left;  // Valor del lado izquierdo de la ficha.
    unsigned short right; // Valor del lado derecho de la ficha.
};


/**
 * Sobrecarga del operador de salida.
 */
ostream &operator<<(ostream &os, const DominoTile &d);

/**
 * Sobrecarga del operador de entrada.
 * Se lee la ficha teniendo en cuenta el formato |left|right|
 */
istream &operator>>(istream &stream, DominoTile &d);


#endif //DOMINOES_OVER_TCP_DOMINOTILE_H
