#include <stdio.h>
#include <stdlib.h>
#include "functions.h"


int main(int argc, char const *argv[]) {
    int aristas;
    int nodos;
    int** g = leerArchivo("entrada.in", &nodos, &aristas);
    printf("\n----- IMPRIMIENDO GRAFO -----\n\n");
    imprimirGrafo(g, aristas);
    printf("aristas: %d ; nodos: %d\n\n", aristas, nodos);
    int* arregloNodos = identificarNodos(g, aristas, nodos);
    //printf("%d", arregloNodos[0]);
    bruteForce(arregloNodos, nodos, g, aristas);
    printf("\n----- FIN DEL PROGRAMA -----\n\n");
    free(arregloNodos);
    liberarGrafo(g, aristas);
    return 0;
}