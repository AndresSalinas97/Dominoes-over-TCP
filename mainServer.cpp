/**
 * @file    mainServer.cpp
 * @author  Andrés Salinas Lima
 * @date    07/10/2019
 * @brief   Main del servidor TCP para el juego del Dominó: Gestiona clientes,
 *          usuarios, partidas y la lógica del juego.
 */


#include "DominoesServer.h"
#include "Socket.h"

#include <cstdlib>


// TODO: Capturar señales (ctrl+c)
int main() {
    // Abrimos el socket y lo habilitamos para que pueda recibir conexiones
    Socket socket;
    socket.prepareServerSocket();

    // Iniciamos el servidor
    DominoesServer dominoesServer(socket);
    dominoesServer.start();

    socket.close();

    return EXIT_SUCCESS;
}
