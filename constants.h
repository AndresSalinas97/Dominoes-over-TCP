/**
 * @file    constants.h
 * @author  Andrés Salinas Lima
 * @date    11/10/2019
 * @brief   Definiciones de constantes para el proyecto.
 */


#ifndef DOMINOES_OVER_TCP_CONSTANTS_H
#define DOMINOES_OVER_TCP_CONSTANTS_H


#include <string>


using namespace std;


#define SERVER_PORT 2050 /** Puerto en el que el servidor acepta el servicio. */

#define MSG_SIZE 250     /** Longitud máxima de los mensajes entre cliente y servidor. */

#define MAX_CLIENTS 5    /** Número máximo clientes conectados al servidor. */
#define MAX_DOMINOES 2   /** Número máximo de partidas simultáneas. */

const string USERS_FILE_PATH = "./users.csv"; /** Path del fichero donde se almacenan
                                                  los usuarios y contraseñas en TEXTO
                                                  PLANO (separado por comas). */


#endif //DOMINOES_OVER_TCP_CONSTANTS_H
