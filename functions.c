#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

/**
 * Calcula el factorial de un numero
 * retorna un entero
 */
int factorial(int n){
    if (n == 0) return 1;
    else return n*factorial(n-1);
}

/**
 * Calcula el coeficiente binomial "n sobre k"
 * retorna un entero
 */
int C(int n, int k){
    return factorial(n)/(factorial(n-k)*factorial(k));
}

/**
 * Lee el archivo de entrada
 * Retorna un arreglo con el grafo.
 */
int** leerArchivo(char* path, int* cantidadNodos, int* cantidadAristas){
    int i = 0;
    int** g;
    // Abrir el archivo
    FILE* f = fopen(path, "r");
    if (f == NULL) printf("Error al abrir el archivo");
    // Leer la cantidad de nodos
    fscanf(f, "%d", cantidadNodos);
    *cantidadAristas = C(*cantidadNodos, 2);
    // Salto de linea pendiente
    fgetc(f);
    g = (int**)malloc(sizeof(int*)*(*cantidadAristas));
    // Leer los pares conectados y sus costos
    while (i < *cantidadAristas){
        g[i] = (int*)malloc(sizeof(int)*3);
        // Origen, destino, costo
        fscanf(f, "%d %d %d\n", &g[i][0], &g[i][1], &g[i][2]);
        i++;
    }
    fclose(f);
    return g;
}

void imprimirGrafo(int** g, int l){
    int i = 0;
    while (i < l){
        printf("%d %d %d\n", g[i][0], g[i][1], g[i][2]);
        i++;
    }
}

/**
 * Imprime el estado actual del proceso
 */
void printCurrent(int* array, int l, int costo){
    #ifdef DEBUG
    int i = 0;
    printf("0 ");
    while (i < l){
        printf("%d ", array[i]);
        i++;
    }
    printf("0 ; Costo: %d", costo);
    printf("\n");
    printf("enter para continuar...\n");
    while (getchar() != '\n'){
    }
    #endif
}

/**
 * Muestra el camino y costo optimo que soluciona el problema
 */
void mostrarResultado(int* camino, int l, int costo){
    int i = 0;
    while (i < l){
        printf("%d ", camino[i]);
        i++;
    }
    printf("; Costo: %d\n", costo);
}

/**
 * Escribe el resultado en un archivo
 */
void exportarResultado(int* camino, int l, int costo){
    FILE* f = fopen("salida.out", "w");
    if (f == NULL){
        printf("Error al escribir el archivo\n");
        exit(-1);
    }
    int i = 0;
    fprintf(f, "%d\n", costo);
    while (i < l){
        fprintf(f, "%d ", camino[i]);
        i++;
    }
    fclose(f);
}

/**
 * Busca en el grafo g, el costo entre ir desde a hasta b o viceversa
 * retorna un entero
 */
int costo(int a, int b, int** g, int numAristas){
    int i = 0;
    while (i < numAristas){
        if (g[i][0] == a && g[i][1] == b){
            return g[i][2];
        }
        if (g[i][0] == b && g[i][1] == a){
            return g[i][2];
        }
        i++;
    }
}
/**
 * Calcula el costo total entre un conjunto de nodos, en el orden en el que aparecen
 * retorna un entero
 */
int costoTotal(int* nodos, int cantidadNodos, int** g, int numAristas){
    int costoTotal = 2;
    int a, b;
    int i = 0;
    while (i < cantidadNodos-1){
        a = nodos[i];
        b = nodos[i+1];
        costoTotal += costo(a, b, g, numAristas);
        i+=1;
    }
    return costoTotal;
}
/**
 * Copia el contenido de un conjunto de vertices en dest
 */
void volcarCamino(int* array, int l, int* dest){
    int i = 1;
    while (i <= l){
        dest[i] = array[i-1];
        i++;
    }
}

/**
 * Genera las combinaciones posibles de caminos, siempre partiendo y llegando a la ciudad "0" y resuelve
 * el problema con fuerza bruta.
 * Entradas: int* nodos -> conjunto de nodos totales del grafo (puntos de recoleccion)
 *           int numNodos -> cantidad de nodos totales del grafo (puntos de recoleccion)
 *           int** g -> grafo del problema, representado como una lista de conjuntos de la forma:
 *                      (origen, destino, costo)
 *           int numAristas -> cantidad de aristas que contiene el grafo
 */
void bruteForce(int* nodos, int numNodos, int** g, int numAristas)
{
        int* caminoOptimo = (int*)malloc(sizeof(int)*(numNodos+2));
        caminoOptimo[0] = 0;
        caminoOptimo[numNodos-1] = 0;
        volcarCamino(nodos, numNodos, caminoOptimo);
        int mejorCosto = costoTotal(nodos, numNodos, g, numAristas);
        printCurrent(nodos, numNodos, mejorCosto);
        int scratch;
        int lastpermutation = 0;
        int i, j, k, l;
        while (!lastpermutation)
        {
                //find largest j such that set[j] < set[j+1]; if no such j then done
                j = -1;
                for (i = 0; i < numNodos-1; i++)
                {
                        if (nodos[i+1] > nodos[i])
                        {
                                j = i;
                        }
                }
                if (j == -1)
                {
                        lastpermutation = 1;
                }
                if (!lastpermutation)
                {
                        for (i = j+1; i < numNodos; i++)
                        {
                                if (nodos[i] > nodos[j])
                                {
                                        l = i;
                                }
                        }
                        scratch = nodos[j];
                        nodos[j] = nodos[l];
                        nodos[l] = scratch;
                        //reverse j+1 to end
                        k = (numNodos-1-j)/2; // number of pairs to swap
                        for (i = 0; i < k; i++)
                        {
                                scratch = nodos[j+1+i];
                                nodos[j+1+i] = nodos[numNodos-1-i];
                                nodos[numNodos-1-i] = scratch;
                        }
                        int costo = costoTotal(nodos, numNodos, g, numAristas);
                        printCurrent(nodos, numNodos, costo);
                        if (costo < mejorCosto){
                            volcarCamino(nodos, numNodos, caminoOptimo);
                            mejorCosto = costo;
                        }
             }
        }
        printf("Camino optimo: ");
        mostrarResultado(caminoOptimo, numNodos+2, mejorCosto);
        exportarResultado(caminoOptimo, numNodos+2, mejorCosto);
        free(caminoOptimo);
}

/**
 * Verifica si un entero esta en un arreglo
 */
int estaEn(int a, int* b, int tam){
    int i = 0;
    while (i < tam){
        if (&b[i] != NULL && b[i] == a) return 1;
        i++;
    }
    return 0;
}

/**
 * Crea una lista con todos los nodos del grafo
 * retorna un arreglo de enteros
 */
int* identificarNodos(int** g, int cantidadAristas, int cantidadNodos){
    int* nodos = (int*)malloc(sizeof(int)*cantidadNodos);
    int i = 0;
    int j = 0;
    while(i < cantidadAristas){
        // Si no esta en nodos
        if (nodos != NULL && estaEn(g[i][0], nodos, cantidadNodos) == 0){
            nodos[j] = g[i][0];
            j++;
        }
        if (nodos != NULL && estaEn(g[i][1], nodos, cantidadNodos) == 0){
            nodos[j] = g[i][1];
            j++;
        }
        i++;
    }
    return nodos;
}

void liberarGrafo(int** g, int numAristas){
    int i = 0;
    while (i < numAristas){
        free(g[i]);
        i++;
    }
    free(g);
}