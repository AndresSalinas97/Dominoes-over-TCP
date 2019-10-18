/**
 * @file    mainServer.cpp
 * @author  Andrés Salinas Lima
 * @date    07/10/2019
 * @brief   Main del servidor TCP para el juego del Dominó: Gestiona clientes,
 *          usuarios, partidas y la lógica del juego.
 */


#include "DominoesServer.h"
#include "Socket.h"

#include <csignal>


DominoesServer *dominoesServerPointer; // Para poderlo usar en signalHandler()


void signalHandler(int);


int main() {
    // Abrimos el socket y lo habilitamos para que pueda recibir conexiones
    Socket socket;
    socket.prepareServerSocket();

    // Creamos el objeto dominoesServer e inicializamos dominoesServerPointer
    // para que se pueda usar en signalHandler()
    DominoesServer dominoesServer(socket);
    dominoesServerPointer = &dominoesServer;

    // Capturamos la señal SIGINT (Ctrl+c)
    signal(SIGINT, signalHandler);

    // Iniciamos el servidor
    dominoesServer.start();
}


void signalHandler(int) {
    dominoesServerPointer->end();
}
