# Dominoes over TCP

**Práctica 2 de la Asignatura de Redes.**

Juego del dominó para dos jugadores implementado en C++ con un programa servidor y otro cliente que se comunican por TCP de manera que el servidor puede soportar múltiples partidas simultáneas entre varios clientes.

## Autores

* Andrés Salinas Lima

## Getting Started

Estas instrucciones le proporcionarán una copia del proyecto en funcionamiento en su máquina local para fines de desarrollo y prueba.

### Compilar

La forma más fácil de compilar ambos programas (cliente y servidor) es abrir un terminal en el directorio raíz del proyecto y ejecutar lo siguiente:

```
$ cmake .
$ make
```

### Ejecutar programa servidor

Una vez compilado, para ejecutar el servidor simplemente ejecutamos:

```
$ ./Dominoes_over_TCP_Server
```

### Ejecutar programa cliente

Abrimos una nueva terminal en el mismo directorio y ejecutamos:

```
$ ./Dominoes_over_TCP_Client 127.0.0.1
```

Donde *127.0.0.1* es la dirección IP del servidor (en este caso usamos la dirección IP de loopback porque lo estamos ejecutando todo en local).

## Peculiaridades

* Código en inglés. Comentarios y entrada/salida en español.
* Inicia la partida el jugador con la mejor ficha. Esta ficha se coloca automáticamente y se le pasa el turno al oponente.
* Para elegir la mejor ficha se le asigna una "puntuación" numérica a cada ficha y luego se elige la ficha con la mayor "puntuación".
* Si no es necesario robar ficha, al jugador no se le permitirá robar ficha.
* Si no es necesario pasar el turno, al jugador no se le permitirá pasar el turno.
* Se contempla que la partida pueda quedar en empate.
* Se añade un nuevo tipo de mensaje `*INFO.` que muestra información adicional a los clientes.
* Los usuarios y contraseñas se guardan en un documento en texto plano separado por comas y saltos de linea (`.csv`).
* Se añade un nuevo comando `AYUDA` para los clientes que les envía mensajes de ayuda contextual a los clientes.
* En el programa servidor también se añade un comando `AYUDA` y un comando `STATS` que muestra estadísticas (usuarios conectados, partidas en curso...).

## Resumen del diseño

### Programas principales

* `mainServer.cpp`: Main del servidor TCP para el juego del Dominó: Gestiona clientes, usuarios, partidas y la lógica del juego.
* `mainClient.cpp`: Main del Cliente para servidor TCP: Envía y recibe cadenas de texto.

### Clases

* `Socket`: Base para las clases ServerSocket y ClientSocket.
* `ClientSocket`: Abre, habilita y cierra sockets cliente.
* `ServerSocket`: Abre, habilita y cierra sockets para el servidor.
* `DominoesServer`: Clase para gestionar el servidor del juego del Dominó.
* `User`: Representa a los usuarios conectados.
* `UsersManager`: Gestiona la lista de usuarios conectados y les permite  registrarse e iniciar sesión.
* `DominoTile`: Representa una ficha del dominó.
* `DominoesBoard`: Tablero del juego del dominó. Gestiona las fichas que están sobre la mesa y contiene funciones auxiliares para facilitar la gestión del juego al servidor.

### Otros

* `constants.h`: Definiciones de constantes para el proyecto.

## Especificación original de la práctica

* La comunicación entre los clientes del juego del dominó se realizará bajo el protocolo de transporte TCP. La práctica que se propone consiste en la realización de una aplicación cliente/servidor que implemente el juego del dominó con algunas restricciones. En el juego considerado los jugadores (los clientes) se conectan al servicio (el  servidor).  Solamente se admitirán partidas con dos jugadores en solitario (no se tendrá en cuenta partidas con un número mayor de jugadores, ni la consideración del juego por parejas). En el momento que existan dos jugadores conectados podrán comenzar una partida. Se admiten hasta 10 partidas simultáneas, y hasta 30 jugadores conectados simultáneamente en el servidor.

* El procedimiento que se seguirá será el siguiente:
  * Un cliente se conecta al servicio y si la conexión ha sido correcta el sistema devuelve `+0k. Usuario conectado`.
  * Para poder acceder a los servicios es necesario identificarse mediante el envío del usuario y clave para que el sistema lo valide. Los mensajes que deben indicarse son: `USUARIO usuario` para indicar el usuario, tras el cual el servidor enviará `+Ok. Usuario correcto` o `–ERR. Usuario incorrecto`. En caso de ser correcto el siguiente mensaje que se espera recibir de dicho usuario es `PASSWORD password`, donde el servidor responderá con el mensaje de `+Ok. Usuario validado` o `–ERR. Error en la validación`.
  * El control de usuarios y claves en el servidor se llevará mediante un fichero de texto plano y no se codificará ningún tipo de encriptación.
  * Un usuario nuevo podrá registrarse mediante el mensaje `REGISTRO –u usuario –p password`. Se llevará un control para evitar colisiones con los nombres de usuarios ya existentes.
  * Una vez conectado y validado, el cliente podrá llevar a cabo una partida en el juego indicando un mensaje de `INICIAR-PARTIDA`. Recibido este mensaje en el servidor, éste se encargará de comprobar las personas que tiene pendiente para comenzar una partida.
    * Si con esta petición, ya se forma un grupo de dos jugadores, mandará un mensaje a cada uno de ellos, para indicarle que la partida va a comenzar `+Ok. Empieza la partida`. El orden de los usuarios en el juego será el mismo orden en el que se ha realizado la conexión.
    * Si todavía falta un jugador para iniciar la partida, mandará un mensaje al nuevo usuario, especificando que tiene su petición y que está a la espera de la conexión de otro jugador `+Ok. Petición Recibida. Quedamos a la espera de más jugadores`.
  * Una vez comenzada la partida, se reparten al azar 7 fichas a cada jugador, para ello se manda a cada jugador el mensaje `FICHAS |2|1||1|1||1|4||4|4||1|3||1|4||1|3|`. El jugador que comience será el que tenga el seis doble, teniendo en cuenta que sobre la mesa quedan catorce fichas, puede suceder que ninguno de ellos tenga el seis doble; en este supuesto jugará el que tenga el cinco doble o, en su defecto, el que tenga el doble mayor. Si se diera la posibilidad de que ningún jugador tuviera un doble, saldría el poseedor del 6-5, y así sucesivamente. El servidor será el que indique el turno al jugador oportuno, con el mensaje `+Ok. Turno de partida`, al otro jugador le indicará que es el turno del compañero `+Ok. Turno del otro jugador`.
  * En su turno, cada jugador colocará una de las fichas que tiene disponible con la restricción de que dos fichas solamente pueden colocarse juntas cuando los cuadrados adyacentes son del mismo valor. La sintaxis para colocar una ficha será `COLOCAR-FICHA |valor1|valor2|,extremo`. Siendo valor1 y valor2, los valores que tiene la ficha que se va a enviar y extremo indicará si la va a colocar a la derecha o a la izquierda.
    * Cada vez que un jugador envíe una ficha, el servidor reenviará la información del nuevo tablero a cada uno de los jugadores. El tablero será una cadena de texto formada por la secuencia de fichas que representan el estado de la partida. Por ejemplo, el mensaje `TABLERO |2|1||1|1||1|4||4|4|` representa que hay cuatro fichas colocadas, concretamente, las fichas |2|1|, |1|1|, |1|4| y |4|4|. Por tanto, el jugador deberá tratar de poner una ficha que tenga como valor un 2 (por la izquierda) o un 4 (por la derecha).
  * En caso de que la ficha no pueda colocarse, el servidor devolverá: `-Err. La ficha no puede ser colocada`.
  * Si un jugador no puede colocar ninguna ficha en su turno, robará de las del montón que ha quedado en la mesa, hasta que consiga alguna que le permita jugar. La sintaxis para robar una ficha del montón será `ROBAR-FICHA`. El servidor devolverá el mensaje `FICHA |valor1|valor2|`, que le enviará una ficha elegida al azar al jugador que la ha solicitado. Si a pesar de haber robado todas las fichas del montón no encuentra ninguna con la que jugar, pasará y será el otro jugador quien continúe el juego. El servidor controlará, si realmente no puede poner ficha. En caso de que tenga opción de colocar una ficha, avisará al usuario `+Ok. No es necesario robar ficha.`.
  * Si un jugador no puede colocar ninguna ficha en su turno, y no quedan fichas disponibles en el montón, tendrá que pasar el turno al siguiente jugador, con el mensaje `PASO-TURNO`. El servidor controlará, si realmente no puede poner ficha. En caso de que tenga opción de colocar una ficha, avisará al usuario `+Ok. No es necesario pasar turno.`.
  * El mensaje para indicar que es el turno de un jugador será enviado por el servidor al jugador específico mediante el mensaje: `+Ok. Turno de partida`.
  * La partida continúa con los jugadores colocando sus fichas hasta que se presenta alguna de las situaciones siguientes:
    * Cuando un jugador coloca su última ficha en la mesa, el jugador gana la partida. El servidor mandará un mensaje indicando `+Ok. Partida Finalizada`. JugadorN ha ganado la partida`, siendo jugadorN el identificador del jugador que ha ganado.
    * Todos los jugadores tienen fichas, pero ninguno de ellos puede continuar la partida. Esto ocurre cuando los números de los extremos ya han sido jugados 7 veces. En ese momento la partida está cerrada. Los jugadores contarán los puntos de las fichas que les quede; el jugador con menos puntos será el ganador. `+Ok. Partida Finalizada. JugadorN ha ganado la partida`, siendo jugadorN el identificador del jugador que ha ganado.
  * Un jugador siempre podrá salir de la aplicación en cualquier momento. De este modo, el comando `SALIR` al servidor, implicará que si estaba esperando para comenzar una partida, debe eliminarse de la espera de dicha partida y se le mandará un mensaje `+Ok. Desconexión procesada`. En caso de estar jugando una partida se anulará dicha partida, dejando un mensaje a todos los jugadores `+Ok. La partida ha sido anulada` y toda la información relativa a dicha partida será eliminada.
  * Cualquier mensaje que no use uno de los especificadores detallados, generará un mensaje de `-ERR` por parte del servidor.

* Algunas restricciones a tener en cuenta en el juego son:
  * La comunicación será mediante consola.
  * Para representar las fichas de dominó se utilizar el formato `|valor1|valor2|`, donde valor1 y valor2 son valores concretos de cada extremo de la ficha (las barras verticales actúan como delimitadores de la información de una ficha). Obviamente, valor1 y valor2 deberá tener un valor numérico comprendido entre 0 y 6.
  * De acuerdo con el formato anterior, el mensaje enviado por el servidor al cliente con sus fichas iniciales será una secuencia de fichas, de acuerdo con el formato anterior. Por ejemplo, el mensaje `|2|3||5|2||3|0|` representa que las fichas |2|3|, |5|2| y |3|0| han sido repartidas al jugador.
  * El cliente deberá aceptar como argumento una dirección IP que será la dirección del servidor al que se conectará.
  * El protocolo deberá permitir mandar mensajes de tamaño arbitrario. Teniendo como tamaño máximo de envío una cadena de longitud 250 caracteres.
  * El servidor aceptará servicios en el puerto 2050.
  * El servidor debe permitir la conexión de varios clientes simultáneamente. Se utilizará la función select() para permitir esta posibilidad.
  * El número máximo de clientes conectados será de 40 usuarios. Lo que supondrá 10 partidas simultáneas.
  * Todos los mensajes mandados al servidor con respecto a la conexión y validación o la creación de canales recibirán una respuesta indicando que ha sido correcto `+OK. Texto informativo` o que ha habido algún error `-ERR. Texto informativo`.
 
* Resumen de los tipos de mensajes:
    * `USUARIO usuario`: mensaje para introducir el usuario que desea conectarse.
    * `PASSWORD contraseña`: mensaje para introducir la contraseña asociada al usuario.
    * `REGISTRO –u usuario –p password –d Nombre y Apellidos –c Ciudad`: mensaje mediante el cual el usuario solicita registrarse para acceder al servicio de chat que escucha en el puerto TCP 2050.
    * `PASO-TURNO`: mensaje para indicar que no se dispone de ninguna ficha para colocar en ese turno.
    * `INICIAR-PARTIDA`: mensaje para indicar el interés en jugar una partida de dominó.
    * `COLOCAR-FICHA |valor1|valor2|,extremo`: mensaje para colocar una ficha en el tablero.
    * `FICHAS |valor1|valor2||…||`: mensaje para enviar las fichas de un jugador.
    * `TABLERO |valor1|valor2||…||`: mensaje para enviar el tablero de la partida.
    * `ROBAR-FICHA`: mensaje para robar una ficha del montón.
    * `FICHA |valor1|valor2|`: mensaje para enviar una ficha a un jugador que haya robado.
    * `SALIR`: mensaje para solicitar salir del juego.
    * Cualquier otra línea que se escriba no será reconocida por el protocolo como un mensaje válido y generar su correspondiente `-Err.` por parte del servidor.
