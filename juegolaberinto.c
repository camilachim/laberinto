#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PARED '+'
#define CAMINO ' '
#define JUGADOR 'o'
#define SALIDA 'x'
#define FILAS 13
#define COLUMNAS 13

typedef struct {
    int x, y;
} Punto;

void inicializar(char laberinto[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            laberinto[i][j] = PARED;
        }
    }
}

void limpiarPantalla() {
    printf("\033[2J");  // Secuencia de escape ANSI para limpiar la pantalla
    printf("\033[H");   // Secuencia de escape ANSI para mover el cursor a la posición (0, 0)
}

void imprimir(char laberinto[FILAS][COLUMNAS], Punto jugador, FILE *archivo) {
    limpiarPantalla();
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            if (i == jugador.y && j == jugador.x) {
                printf("%c ", JUGADOR);
                fprintf(archivo, "%c ", JUGADOR);
            } else {
                printf("%c ", laberinto[i][j]);
                fprintf(archivo, "%c ", laberinto[i][j]);
            }
        }
        printf("\n");
        fprintf(archivo, "\n");
    }
}

void moverJugador(char direccion, Punto *jugador, char laberinto[FILAS][COLUMNAS]) {
    Punto nuevoJugador = *jugador;
    switch (direccion) {
        case 'w': nuevoJugador.y--; break;
        case 's': nuevoJugador.y++; break;
        case 'a': nuevoJugador.x--; break;
        case 'd': nuevoJugador.x++; break;
    }

    if (nuevoJugador.y >= 0 && nuevoJugador.y < FILAS && nuevoJugador.x >= 0 && nuevoJugador.x < COLUMNAS) {
        if (laberinto[nuevoJugador.y][nuevoJugador.x] == CAMINO || laberinto[nuevoJugador.y][nuevoJugador.x] == SALIDA) {
            *jugador = nuevoJugador;
        }
    }
}

void generarLaberintoDFS(char laberinto[FILAS][COLUMNAS], Punto *salida) {
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

int main() {
    char laberinto[FILAS][COLUMNAS];
    Punto jugador = {1, 1};
    Punto salida;

    inicializar(laberinto);
    generarLaberintoDFS(laberinto, &salida);

    FILE *archivo = fopen("laberinto.txt", "w");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo para escribir.\n");
        return 1;
    }

    while (jugador.x != salida.x || jugador.y != salida.y) {
        imprimir(laberinto, jugador, archivo);
        printf("Mover con W-A-S-D: \n");
        printf("W arriba\n");
        printf("A izquierda\n");
        printf("S abajo\n");
        printf("D derecha\n");
        printf("Ingresa las indicaciones: ");
        char direccion = getchar();
        while (getchar() != '\n');  // Limpiar el buffer de entrada
        
        moverJugador(direccion, &jugador, laberinto);
    }

    printf("¡Felicidades! Has encontrado la salida.\n");

    fclose(archivo);
    return 0;
}