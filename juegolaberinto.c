//Librerias a usar
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Definimos constantes
#define PARED '+'
#define CAMINO ' '
#define JUGADOR 'o'
#define SALIDA 'x'
#define FILAS 13
#define COLUMNAS 13

//Estructura del laberinto en un plano bidimencional
typedef struct {
    int x, y;
} Punto;

//Con esta funcion inicializamos el laberinto
void inicializar(char laberinto[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            laberinto[i][j] = PARED;
        }
    }
}

//Nos ayuda a limpiar la pantalla
void limpiar() {
    printf("\033[2J");
    printf("\033[H");  
}

//Esta imprime el laberiento
void imprimir(char laberinto[FILAS][COLUMNAS], Punto jugador) {
    limpiar();
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (i == jugador.y && j == jugador.x) {
                printf("%c ", JUGADOR);
            } else {
                printf("%c ", laberinto[i][j]);
            }
        }
        printf("\n");
    }
}

//Guarda el laberinto en un archivo
void guardarArchivo(char laberinto[FILAS][COLUMNAS], const char* nombreArchivo, Punto movimientos[], int pasos) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (archivo == NULL) {
        exit(1);
    }

    // Guardar el laberinto
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            fprintf(archivo, "%c ", laberinto[i][j]);
        }
        fprintf(archivo, "\n");
    }

    // Guardar los movimientos en coordenadas
    fprintf(archivo, "Movimientos:\n");
    for (int i = 0; i < pasos; i++) {
        fprintf(archivo, "(%d, %d)\n", movimientos[i].x, movimientos[i].y);
    }

    fprintf(archivo, "Los movimientos realizados fueron: %d\n", pasos - 1);

    fclose(archivo);
}

//Mueve el jugador a la direccion que el usuario proporcione 
void mover(char direccion, Punto *jugador, char laberinto[FILAS][COLUMNAS]) {
    Punto Jugador = *jugador;
    switch (direccion) {
        case 'w': Jugador.y--; break;
        case 's': Jugador.y++; break;
        case 'a': Jugador.x--; break;
        case 'd': Jugador.x++; break;
    }

    if (Jugador.y >= 0 && Jugador.y < FILAS && Jugador.x >= 0 && Jugador.x < COLUMNAS) {
        if (laberinto[Jugador.y][Jugador.x] == CAMINO || laberinto[Jugador.y][Jugador.x] == SALIDA) {
            laberinto[jugador->y][jugador->x] = CAMINO; // Borra la posición anterior del jugador
            *jugador = Jugador;
            laberinto[jugador->y][jugador->x] = JUGADOR; // Actualiza la nueva posición del jugador
        }
    }
}

//Genera el laberinto con los movimientos
void generar(char laberinto[FILAS][COLUMNAS], Punto *salida) {
    int direcciones[4][2] = {{0, 2}, {0, -2}, {2, 0}, {-2, 0}};
    Punto pila[FILAS * COLUMNAS];
    int tope = 0;

    int x = 1, y = 1;
    pila[tope++] = (Punto){x, y};
    laberinto[y][x] = CAMINO;

    srand(time(0));

    while (tope > 0) {
        Punto actual = pila[tope - 1];
        int movido = 0;
        int d[4] = {0, 1, 2, 3};

        for (int i = 3; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = d[i];
            d[i] = d[j];
            d[j] = temp;
        }

        for (int i = 0; i < 4; i++) {
            int nx = actual.x + direcciones[d[i]][0];
            int ny = actual.y + direcciones[d[i]][1];

            if (nx > 0 && nx < COLUMNAS && ny > 0 && ny < FILAS && laberinto[ny][nx] == PARED) {
                int px = actual.x + direcciones[d[i]][0] / 2;
                int py = actual.y + direcciones[d[i]][1] / 2;
                laberinto[ny][nx] = CAMINO;
                laberinto[py][px] = CAMINO;
                pila[tope++] = (Punto){nx, ny};
                movido = 1;
                break;
            }
        }

        if (!movido) {
            tope--;
        }
    }

    laberinto[FILAS - 2][COLUMNAS - 2] = SALIDA;
    *salida = (Punto){COLUMNAS - 2, FILAS - 2};
}

//Funcion principal con todos los llamados a las funciones o procedimientos 
int main() {
    char laberinto[FILAS][COLUMNAS];
    Punto jugador = {1, 1};
    Punto salida;
    Punto movimientos[FILAS * COLUMNAS];
    int pasos = 0;

    inicializar(laberinto);
    generar(laberinto, &salida);
    laberinto[jugador.y][jugador.x] = JUGADOR;
    movimientos[pasos++] = jugador;

    while (jugador.x != salida.x || jugador.y != salida.y) {
        imprimir(laberinto, jugador);
        printf("Mover con W-A-S-D: \n");
        printf("W arriba\n");
        printf("A izquierda\n");
        printf("S abajo\n");
        printf("D derecha\n");
        printf("Ingresa las indicaciones: ");
        char direccion = getchar();
        while (getchar() != '\n');  // Limpiar el buffer de entrada

        mover(direccion, &jugador, laberinto);
        movimientos[pasos++] = jugador;
    }

    imprimir(laberinto, jugador);
    printf("¡Felicidades! Has encontrado la salida.\n");
    printf("Los movimientos realizados fueron: %d\n", pasos - 1);


    guardarArchivo(laberinto, "solucion.txt", movimientos, pasos);

    return 0;
}
