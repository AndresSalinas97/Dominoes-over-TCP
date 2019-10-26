/**
 * @file    DominoesServer.cpp
 * @author  Andrés Salinas Lima
 * @date    17/10/2019
 * @brief   Código de la clase DominoesServer.
 */


#include "DominoesServer.h"
#include "constants.h"

#include <iostream>
#include <cstring>
#include <vector>
#include <iterator>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>


using namespace std;


void DominoesServer::start() {
    char receivedMessage[MSG_SIZE];
    int nReceived;

    // Mensajes de inicio:
    printHelp();
    cout << "\t* Servidor iniciado *" << endl;

    // Inicializamos los conjuntos fd_set para la función select()
    fd_set auxFDS; // Set de descriptores auxiliar para la función select()
    FD_ZERO(&auxFDS); // Vacía el el set
    FD_ZERO(&readFDS); // Vacía el set
    FD_SET((unsigned) 0, &readFDS); // Añade stdin al set de descriptores de lectura
    FD_SET(serverSocket.getDescriptor(), &readFDS); // Añade el socket del
    // servidor al set de descriptores de lectura

    // Intercambio de mensajes
    while (true) {
        auxFDS = readFDS; // Inicializamos auxFDS antes de llamar a select()

        // select() duerme el proceso hasta que haya datos disponibles en alguno
        // de los sockets del set
        if ((select(FD_SETSIZE, &auxFDS, nullptr, nullptr, nullptr)) < 0) {
            cerr << "Error en select(): " << strerror(errno) << endl;
            break;
        }

        for (int activeSocketD = 0; activeSocketD < FD_SETSIZE; activeSocketD++) {
            // Recorremos todos los descriptores posibles
            if (FD_ISSET(activeSocketD, &auxFDS)) {
                // Tenemos nuevos datos en el socket i
                if (activeSocketD == 0) {
                    // El usuario ha tecleado un mensaje
                    handleUserInput();
                } else if (activeSocketD == serverSocket.getDescriptor()) {
                    // Tenemos nuevos datos en el socket del servidor lo que
                    // significa que tenemos un nuevo cliente
                    handleNewClient();
                } else {
                    // Tenemos nuevos datos en el socket de algún cliente
                    bzero(receivedMessage, sizeof(receivedMessage));
                    nReceived = recv(activeSocketD, receivedMessage,
                                     sizeof(receivedMessage), 0);

                    if (nReceived < 0) {
                        cerr << "Error al recibir mensaje del cliente "
                             << activeSocketD << ": " << strerror(errno) << endl;
                    } else if (nReceived == 0) {
                        // El cliente nos ha abandonado
                        handleGoneClient(activeSocketD);
                    } else {
                        // El cliente nos envía un mensaje
                        handleClientCommunication(activeSocketD, string(receivedMessage));
                    }
                }
            }
        }

        fflush(stdout);
        fflush(stderr);
    }

    // El servidor termina cerrando el socket
    serverSocket.close();
}

void DominoesServer::end() {
    cout << "\t* El servidor se cerrará después de avisar a los clientes *" << endl;

    for (auto &user : usersManager.getUsers())
        sendMessage(user.getSocketDescriptor(), "*INFO. El servidor va a cerrar");

    serverSocket.close();

    exit(EXIT_SUCCESS);
}

void DominoesServer::sendMessage(int destinationSocketD, const char *message) {
    // Para asegurar que no se sobrepasa MSG_SIZE
    char sentMessage[MSG_SIZE];
    bzero(sentMessage, sizeof(sentMessage));
    strcpy(sentMessage, message);

    // Añadimos un \n al final del mensaje enviado
    strcat(sentMessage, "\n");

    if ((send(destinationSocketD, sentMessage, strlen(sentMessage), 0)) < 0)
        cerr << "Error al enviar mensaje al cliente " << destinationSocketD
             << ": " << strerror(errno) << endl;
}

void DominoesServer::handleUserInput() {
    string input;

    cin >> input;
    cin.clear();
    cin.ignore();

    if (input == "SALIR")
        end();
    else if (input == "STATS")
        printStats();
    else if (input == "AYUDA")
        printHelp();
    else
        cout << "\t* Comando no válido *" << endl;
}

void DominoesServer::handleNewClient() {
    int newClientSocketD;
    struct sockaddr_in clientAddress = {};
    socklen_t clientAddressLength = sizeof(clientAddress);

    // Aceptamos la conexión obteniendo el socket del nuevo cliente
    if ((newClientSocketD = accept(serverSocket.getDescriptor(),
                                   (struct sockaddr *) &clientAddress,
                                   &clientAddressLength)) < 0) {
        cerr << "Error en accept(): " << strerror(errno) << endl;
    } else {
        // Comprobamos que no hayamos superado el número máximo de clientes conectados
        if (usersManager.getNUsers() >= MAX_CLIENTS) {
            sendMessage(newClientSocketD, "-ERR. Se ha superado el número máximo"
                                          " de usuarios conectados");
            close(newClientSocketD);
            cout << "\t* Cliente rechazado en socket " << newClientSocketD
                 << " *" << endl;
        } else {
            // Tenemos un nuevo cliente

            // Añadimos el nuevo cliente a la lista de clientes
            usersManager.addUser(newClientSocketD);

            FD_SET(newClientSocketD, &readFDS);

            cout << "\t* Nuevo cliente en socket " << newClientSocketD << " *" << endl;

            sendHelp(newClientSocketD);

            sendMessage(newClientSocketD, "+OK. Usuario conectado");
        }
    }
}

void DominoesServer::handleGoneClient(int goneClientSocketD) {
    cout << "\t* Cliente desconectado en socket " << goneClientSocketD << " *" << endl;

    removeClient(goneClientSocketD);
}

void DominoesServer::removeClient(int goneClientSocketD) {
    User goneUser = usersManager.getUser(goneClientSocketD);

    // Si el usuario estaba jugando
    if (goneUser.isPlaying()) {
        // Sacamos al oponente de la partida
        goneUser.getOpponent()->resetLoggedInUserState();

        // Notificamos al oponente
        sendMessage(goneUser.getOpponent()->getSocketDescriptor(),
                    "+OK. La partida ha sida anulada");

        // Eliminamos el tablero de dominó
        removeDominoesBoard(goneUser.getDominoesBoard());
    }

    // Se elimina de la lista de clientes
    usersManager.removeUser(goneClientSocketD);

    // Se cierra su socket
    close(goneClientSocketD);

    // Se elimina su socket de readFDS
    FD_CLR(goneClientSocketD, &readFDS);
}

void DominoesServer::removeDominoesBoard(DominoesBoard *dominoesBoard) {
    for (auto it = dominoesBoards.begin(); it != dominoesBoards.end(); it++) {
        if (dominoesBoard == &*it) {
            dominoesBoards.erase(it);
            return;
        }
    }
}

void DominoesServer::handleClientCommunication(int clientSocketD,
                                               const string &receivedMessage) {
    // Separamos receivedMessage en palabras
    std::istringstream iss(receivedMessage);
    vector<string> tokens{std::istream_iterator<string>{iss},
                          std::istream_iterator<string>{}};

    if (tokens.empty()) {
        sendMessage(clientSocketD, "-ERR. Mensaje no válido");
        return;
    }

    if (tokens[0] == "USUARIO") {
        if (tokens.size() != 2)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handleUsuarioCommand(clientSocketD, tokens[1]);

    } else if (tokens[0] == "PASSWORD") {
        if (tokens.size() != 2)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handlePasswordCommand(clientSocketD, tokens[1]);

    } else if (tokens[0] == "REGISTRO") {
        if (tokens.size() != 5 || tokens[1] != "-u" || tokens[3] != "-p")
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handleRegistroCommand(clientSocketD, tokens[2], tokens[4]);

    } else if (tokens[0] == "INICIAR-PARTIDA") {
        if (tokens.size() != 1)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handleIniciarPartidaCommand(clientSocketD);

    } else if (tokens[0] == "COLOCAR-FICHA") {
        if (tokens.size() != 2)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handleColocarFichaCommand(clientSocketD, tokens[1]);

    } else if (tokens[0] == "PASO-TURNO") {
        if (tokens.size() != 1)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handlePasoTurnoCommand(clientSocketD);

    } else if (tokens[0] == "ROBAR-FICHA") {
        if (tokens.size() != 1)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handleRobarFichaCommand(clientSocketD);

    } else if (tokens[0] == "AYUDA") {
        if (tokens.size() != 1)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            sendHelp(clientSocketD);

    } else if (tokens[0] == "SALIR") {
        if (tokens.size() != 1)
            sendMessage(clientSocketD, "-ERR. Formato no válido");
        else
            handleSalirCommand(clientSocketD);

    } else {
        sendMessage(clientSocketD, "-ERR. Comando no válido");
    }
}

void DominoesServer::printStats() const {
    cout << "\t* ESTADÍSTICAS *" << endl;

    cout << "\t\tClientes conectados: " << usersManager.getNUsers() << endl;
    cout << "\t\tPartidas en curso: " << dominoesBoards.size() << endl;
    cout << "\t\tClientes registrados: " << usersManager.getNRegisteredUsers() << endl;
}

void DominoesServer::printHelp() {
    cout << "\t* COMANDOS DISPONIBLES *" << endl;
    cout << "\t\t- STATS: Mostrar estadísticas" << endl;
    cout << "\t\t- SALIR: Salir" << endl;
    cout << "\t\t- AYUDA: Mostrar esta ayuda" << endl;
}

void DominoesServer::handleUsuarioCommand(int clientSocketD, const string &username) {
    User user = usersManager.getUser(clientSocketD);

    if (user.isPasswordLogged()) {
        sendMessage(clientSocketD, "-ERR. Ya has iniciado sesión");
        return;
    }

    if (usersManager.loginUsername(clientSocketD, username))
        sendMessage(clientSocketD, "+OK. Usuario correcto");
    else
        sendMessage(clientSocketD, "-ERR. Usuario incorrecto");
}

void DominoesServer::handlePasswordCommand(int clientSocketD, const string &password) {
    User user = usersManager.getUser(clientSocketD);

    if (user.isPasswordLogged()) {
        sendMessage(clientSocketD, "-ERR. Ya has iniciado sesión");
        return;
    }

    if (!user.isUsernameLogged()) {
        sendMessage(clientSocketD, "-ERR. Todavía no has introducido el nombre de"
                                   " usuario");
        return;
    }

    if (usersManager.loginPassword(clientSocketD, password))
        sendMessage(clientSocketD, "+OK. Usuario validado");
    else
        sendMessage(clientSocketD, "-ERR. Error en la validación");
}

void DominoesServer::handleRegistroCommand(int clientSocketD, const string &username,
                                           const string &password) {
    User user = usersManager.getUser(clientSocketD);

    if (user.isPasswordLogged()) {
        sendMessage(clientSocketD, "-ERR. Ya has iniciado sesión");
        return;
    }

    if (usersManager.registerUser(clientSocketD, username, password)) {
        sendMessage(clientSocketD, "+OK. Usuario registrado y sesión iniciada.");
        cout << "\t* Nuevo usuario registrado: " << username << " *" << endl;
    } else {
        sendMessage(clientSocketD, "-ERR. Error en el registro. Pruebe con otro "
                                   "nombre de usuario");
    }
}

void DominoesServer::handleIniciarPartidaCommand(int clientSocketD) {
    User *user = usersManager.getUserPtr(clientSocketD);

    if (!user->isPasswordLogged()) {
        sendMessage(clientSocketD, "-ERR. Todavía no has iniciado sesión");
        return;
    }

    if (user->isWaiting()) {
        sendMessage(clientSocketD, "-ERR. Ya estás en la lista de espera");
        return;
    }

    if (user->isPlaying()) {
        sendMessage(clientSocketD, "-ERR. Ya estás jugando");
        return;
    }

    if (dominoesBoards.size() >= MAX_DOMINOES) {
        sendMessage(clientSocketD, "-ERR. Se ha superado el número máximo de "
                                   "partidas");
        return;
    }

    // Comprobamos si hay otros usuarios esperando para jugar
    for (auto &opponent : usersManager.getUsers())
        if (opponent.isWaiting() && opponent.getSocketDescriptor() != clientSocketD) {
            // Hemos encontrado un oponente

            ostringstream os;

            // Creamos el tablero
            dominoesBoards.push_back(*(new DominoesBoard));
            DominoesBoard *dominoesBoard = &dominoesBoards.back();

            // Actualizamos el estado del usuario
            user->setDominoesBoard(dominoesBoard);
            user->setOpponent(&opponent);
            user->setPlaying(true);

            // Actualizamos el estado del oponente
            opponent.setDominoesBoard(dominoesBoard);
            opponent.setOpponent(user);
            opponent.setPlaying(true);
            opponent.setWaiting(false);

            // Notificamos a ambos
            sendMessage(clientSocketD, "+OK. Empieza la partida");
            os << "+INFO. Jugarás contra " << opponent.getUsername();
            sendMessage(clientSocketD, os.str().c_str());
            sendMessage(opponent.getSocketDescriptor(), "+OK. Empieza la partida");
            os.str("");
            os.clear();
            os << "+INFO. Jugarás contra " << user->getUsername();
            sendMessage(opponent.getSocketDescriptor(), os.str().c_str());

            // Repartimos las fichas
            dominoesBoard->shuffle(user->getDominoTiles(), opponent.getDominoTiles());

            // Mostramos las fichas a cada jugador
            sendTiles(*user);
            sendTiles(opponent);

            // El jugador con la mejor ficha empieza la partida y esta ficha se
            // coloca automáticamente
            User *firstPlayer = DominoesBoard::whoStarts(user, &opponent);
            DominoTile bestTile = DominoesBoard::getBestDominoTile(
                    firstPlayer->getDominoTiles());

            sendMessage(firstPlayer->getSocketDescriptor(),
                        "+OK. Turno de partida");
            sendMessage(firstPlayer->getSocketDescriptor(),
                        "+INFO. Empiezas la partida porque tienes la mejor ficha");
            sendMessage(firstPlayer->getSocketDescriptor(),
                        "+INFO. Tu mejor ficha se coloca automáticamente");
            os.str("");
            os.clear();
            os << "COLOCAR-FICHA " << bestTile;
            sendMessage(firstPlayer->getSocketDescriptor(), os.str().c_str());

            // Colocamos la ficha
            firstPlayer->getDominoesBoard()->placeTileCenter(bestTile);

            // Eliminamos la ficha que el jugador acaba de colocar
            for (auto it = firstPlayer->getDominoTiles().begin();
                 it != firstPlayer->getDominoTiles().end(); it++)
                if (*it == bestTile) {
                    firstPlayer->getDominoTiles().erase(it);
                    break;
                }

            // Mandamos el tablero recién modificado a ambos jugadores
            sendBoard(*firstPlayer);
            sendBoard(*firstPlayer->getOpponent());

            // Pasamos el turno al oponente
            firstPlayer->getOpponent()->setMyTurn(true);
            sendMessage(firstPlayer->getOpponent()->getSocketDescriptor(),
                        "+OK. Turno de partida");
            sendTiles(*firstPlayer->getOpponent());

            return;
        }

    // Si no encontramos un oponente nos quedamos a la espera
    user->setWaiting(true);
    sendMessage(clientSocketD,
                "+OK. Petición recibida. Quedamos a la espera de más jugadores");
}

void DominoesServer::handleColocarFichaCommand(int clientSocketD,
                                               const string &dominoAndSide) {
    User *user = usersManager.getUserPtr(clientSocketD);
    DominoesBoard *dominoesBoard = user->getDominoesBoard();
    User *opponent = user->getOpponent();

    if (!user->isPlaying()) {
        sendMessage(clientSocketD, "-ERR. Todavía no estás jugando");
        return;
    }

    if (!user->isMyTurn()) {
        sendMessage(clientSocketD, "-ERR. No es tu turno");
        return;
    }

    // Leemos la ficha y el extremo
    DominoTile dominoTile(-1, -1);
    stringstream ss(dominoAndSide);
    string side;

    ss >> dominoTile;
    getline(ss, side, ',');
    getline(ss, side);

    if (side != "izquierda" && side != "derecha") {
        sendMessage(clientSocketD, "ERR. Formato no válido");
        return;
    }

    // Comprobamos que el jugador realmente tenga la ficha que quiere colocar
    bool playerHasTheTile = false;
    for (DominoTile &d : user->getDominoTiles())
        if (d == dominoTile)
            playerHasTheTile = true;
    if (!playerHasTheTile) {
        sendMessage(clientSocketD, "ERR. No tienes esa ficha");
        return;
    }

    // Colocar ficha (incluye comprobación)
    bool ok;
    if (side == "izquierda") {
        ok = dominoesBoard->placeTileLeft(dominoTile);
    } else {
        ok = dominoesBoard->placeTileRight(dominoTile);
    }

    if (!ok) {
        sendMessage(clientSocketD, "ERR. La ficha no puede ser colocada");
        return;
    }

    // Eliminamos la ficha que el jugador acaba de colocar
    for (auto it = user->getDominoTiles().begin(); it != user->getDominoTiles().end(); it++)
        if (*it == dominoTile) {
            user->getDominoTiles().erase(it);
            break;
        }

    // Mandamos el tablero recién modificado a ambos jugadores
    sendBoard(*user);
    sendBoard(*opponent);

    // Comprobamos si tenemos ganador
    if (checkWinners(user, opponent, dominoesBoard))
        return;

    // Pasamos el turno al oponente
    user->setMyTurn(false);
    opponent->setMyTurn(true);
    sendMessage(opponent->getSocketDescriptor(), "+OK. Turno de partida");
    sendTiles(*opponent);
}

void DominoesServer::handlePasoTurnoCommand(int clientSocketD) {
    User *user = usersManager.getUserPtr(clientSocketD);
    DominoesBoard *dominoesBoard = user->getDominoesBoard();
    User *opponent = user->getOpponent();

    if (!user->isPlaying()) {
        sendMessage(clientSocketD, "-ERR. Todavía no estás jugando");
        return;
    }

    if (!user->isMyTurn()) {
        sendMessage(clientSocketD, "-ERR. No es tu turno");
        return;
    }

    if (dominoesBoard->canPlayerPlay(*user)) {
        sendMessage(clientSocketD, "-ERR. No es necesario pasar turno");
        return;
    }

    if (!dominoesBoard->canPlayerPlay(*user) && !dominoesBoard->sleepingTilesIsEmpty()) {
        sendMessage(clientSocketD, "-ERR. Antes debes robar ficha");
        return;
    }

    // Pasamos el turno al oponente
    user->setMyTurn(false);
    opponent->setMyTurn(true);

    // Informamos al oponente
    sendMessage(opponent->getSocketDescriptor(), "+OK. Turno de partida");
    sendTiles(*opponent);
}

void DominoesServer::handleRobarFichaCommand(int clientSocketD) {
    User *user = usersManager.getUserPtr(clientSocketD);
    DominoesBoard *dominoesBoard = user->getDominoesBoard();

    if (!user->isPlaying()) {
        sendMessage(clientSocketD, "-ERR. Todavía no estás jugando");
        return;
    }

    if (!user->isMyTurn()) {
        sendMessage(clientSocketD, "-ERR. No es tu turno");
        return;
    }

    if (dominoesBoard->sleepingTilesIsEmpty()) {
        sendMessage(clientSocketD, "-ERR. No quedan más fichas en el montón");
        return;
    }

    if (dominoesBoard->canPlayerPlay(*user)) {
        sendMessage(clientSocketD, "-ERR. No es necesario robar ficha");
        return;
    }

    // Cogemos la ficha y la añadimos a las fichas del usuario
    const DominoTile &sleepingTile = dominoesBoard->takeSleepingTile();
    user->getDominoTiles().push_back(sleepingTile);

    // Notificamos al usuario
    ostringstream os;
    os << "FICHA " << sleepingTile;
    sendMessage(clientSocketD, os.str().c_str());

    // Comprobamos si tenemos ganador
    checkWinners(user, user->getOpponent(), dominoesBoard);
}

void DominoesServer::sendHelp(int clientSocketD) {
    // Obtenemos el cliente para mostrar ayuda contextual
    User user = usersManager.getUser(clientSocketD);

    sendMessage(clientSocketD, "*INFO. Comandos disponibles para su estado actual:");

    if (!user.isPasswordLogged()) {
        sendMessage(clientSocketD, "\tUSUARIO nombreDeUsuario\n"
                                   "\tPASSWORD contraseña\n"
                                   "\tREGISTRO -u nombreDeUsuario -p contraseña");
    } else if (!user.isWaiting()) {
        sendMessage(clientSocketD, "\tINICIAR-PARTIDA");
    }

    if (user.isMyTurn()) {
        sendMessage(clientSocketD, "\tCOLOCAR-FICHA |valor1|valor2|,extremo"
                                   "(derecha/izquierda)\n"
                                   "\tPASO-TURNO\n"
                                   "\tROBAR-FICHA");
    }

    sendMessage(clientSocketD, "\tAYUDA\n"
                               "\tSALIR");
}

void DominoesServer::sendTiles(User &user) {
    ostringstream os;
    os << "FICHAS ";
    for (auto &dominoTile : user.getDominoTiles())
        os << dominoTile;
    sendMessage(user.getSocketDescriptor(), os.str().c_str());
}

void DominoesServer::sendBoard(User &user) {
    ostringstream os;
    os << "TABLERO ";
    for (auto &dominoTile : user.getDominoesBoard()->getBoardTiles())
        os << dominoTile;
    sendMessage(user.getSocketDescriptor(), os.str().c_str());
}


void DominoesServer::handleSalirCommand(int clientSocketD) {
    sendMessage(clientSocketD, "+OK. Desconexión procesada");

    cout << "\t* Cliente abandona en socket " << clientSocketD << " *" << endl;

    removeClient(clientSocketD);
}

bool DominoesServer::checkWinners(User *user, User *opponent, DominoesBoard *dominoesBoard) {
    // TODO: Comprobar si funciona
    User *winner;
    if (dominoesBoard->checkWinner(user, opponent, &winner)) {
        // La partida ha terminado
        if (winner == nullptr) {
            // Tenemos empate
            sendMessage(user->getSocketDescriptor(),
                        "OK. Partida Finalizada. Tenemos un empate");
            sendMessage(opponent->getSocketDescriptor(),
                        "OK. Partida Finalizada. Tenemos un empate");
        } else {
            // Tenemos ganador
            ostringstream os;
            os << "OK. Partida Finalizada. " << winner->getUsername()
               << " ha ganado la partida";
            sendMessage(winner->getSocketDescriptor(), os.str().c_str());
            sendMessage(winner->getOpponent()->getSocketDescriptor(), os.str().c_str());
        }

        // Terminamos la partida para ambos jugadores
        user->resetLoggedInUserState();
        opponent->resetLoggedInUserState();

        // Eliminamos el tablero de la lista de tableros
        removeDominoesBoard(dominoesBoard);

        return true;
    }

    return false;
}
