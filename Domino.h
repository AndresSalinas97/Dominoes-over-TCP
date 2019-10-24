/**
 * @file    Domino.h
 * @author  Andrés Salinas Lima
 * @date    24/10/2019
 * @brief   Declaración de la clase Domino.
 */


#ifndef DOMINOES_OVER_TCP_DOMINO_H
#define DOMINOES_OVER_TCP_DOMINO_H


#include <iostream>
#include <string>
#include <sstream>


using namespace std;


/**
 * @class Domino: Representa una ficha del dominó.
 */
class Domino {
public:
    /**
     * Constructor: crea una nueva ficha de dominó a partir de los valores para
     * el lado derecho y el izquierdo introducidos.
     */
    inline Domino(unsigned short left, unsigned short right) : left(left),
                                                               right(right) {}

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
        Domino::left = left;
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
        Domino::right = right;
    }

    /**
     * Le da la vuelta a la ficha (intercambia left y right).
     */
    void flip();

private:
    unsigned short left;  // Valor del lado izquierdo de la ficha.
    unsigned short right; // Valor del lado derecho de la ficha.
};


/**
 * Sobrecarga del operador de salida.
 */
ostream &operator<<(ostream &os, const Domino &d);

/**
 * Sobrecarga del operador de entrada.
 * Se lee la ficha teniendo en cuenta el formato |left|right|
 */
istream &operator>>(istream &stream, Domino &d);


#endif //DOMINOES_OVER_TCP_DOMINO_H
