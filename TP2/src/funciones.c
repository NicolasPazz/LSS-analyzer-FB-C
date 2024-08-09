#include "funciones.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NodoIdentificador* crearNodoIdentificador(const char *identificador) {
    NodoIdentificador *nuevoNodoIdentificador = (NodoIdentificador *)malloc(sizeof(NodoIdentificador)); 
    nuevoNodoIdentificador->identificador = strdup(identificador);
    nuevoNodoIdentificador->contador = 1;
    nuevoNodoIdentificador->siguiente = NULL;
    return nuevoNodoIdentificador;
}

void agregarIdentificador(NodoIdentificador **lista, const char *identificador) {
    if (*lista == NULL) {
        *lista = crearNodoIdentificador(identificador);
        return;
    }

    NodoIdentificador *actual = *lista;
    NodoIdentificador *anterior = NULL;

    while (actual != NULL) {
        anterior = actual; //anterior salio con el ultimo identificador : WHILE
        actual = actual->siguiente;
    } 
    
    anterior->siguiente = crearNodoIdentificador(identificador); 

}

//--------

NodoLiteralCadena* crearNodoLiteralCadena(const char *literalCadena) {
    NodoLiteralCadena *nuevoNodoLiteralCadena = (NodoLiteralCadena *)malloc(sizeof(NodoLiteralCadena));
    nuevoNodoLiteralCadena->literalCadena = strdup(literalCadena);
    nuevoNodoLiteralCadena->longitud = strlen(literalCadena); // a chequear
    nuevoNodoLiteralCadena->siguiente = NULL;
    return nuevoNodoLiteralCadena;
}

void agregarLiteralCadena(NodoLiteralCadena **lista, const char *literalCadena) {
    if (*lista == NULL) {
        *lista = crearNodoLiteralCadena(literalCadena);
        return;
    }

    NodoLiteralCadena *actual = *lista;
    NodoLiteralCadena *anterior = NULL;

    //iterar sobre la lista hasta que longitud de nodo actual sea mayor a la longitud del literal cadena
    while (actual != NULL) {
        if (actual->longitud > strlen(literalCadena)) {
            //agregar nodo literal cadena entre anterior y actual
            NodoLiteralCadena *nuevoNodo = crearNodoLiteralCadena(literalCadena);
            nuevoNodo->siguiente = actual;
            anterior->siguiente = nuevoNodo;
            return;            ;
        }
        anterior = actual;
        actual = actual->siguiente;
    }

    anterior->siguiente = crearNodoLiteralCadena(literalCadena);
}

//--

NodoPalabraReservada* crearNodoPalabraReservada(const char *palabraReservada, int linea, int columna) {
    NodoPalabraReservada *nuevoNodoPalabraReservada = (NodoPalabraReservada *)malloc(sizeof(NodoPalabraReservada)); 
    nuevoNodoPalabraReservada->palabraReservada = strdup(palabraReservada);
    nuevoNodoPalabraReservada->linea = linea;
    nuevoNodoPalabraReservada->columna = columna;
    nuevoNodoPalabraReservada->siguiente = NULL;
    return nuevoNodoPalabraReservada;
}

void agregarPalabraReservada(NodoPalabraReservada **lista, int linea, int columna, const char *palabraReservada) {
    NodoPalabraReservada *actual = *lista;
    NodoPalabraReservada *anterior = NULL;

    while (actual != NULL) {
        anterior = actual; 
        actual = actual->siguiente;
    } 
    
    anterior->siguiente = crearNodoPalabraReservada(palabraReservada, linea, columna); 
}

//--

NodoConstanteEntera* crearNodoConstanteEntera(const char *constanteEntera) {
    NodoConstanteEntera *nuevoNodoConstanteEntera = (NodoConstanteEntera *)malloc(sizeof(NodoConstanteEntera));
    nuevoNodoConstanteEntera->constanteEntera = strdup(constanteEntera);
    return nuevoNodoConstanteEntera;
}

void agregarConstanteEntera(NodoConstanteEntera **lista, const char *constanteEntera) {
    NodoConstanteEntera *actual = *lista;
    NodoConstanteEntera *anterior = NULL;

    while (actual != NULL) {
        anterior = actual; 
        actual = actual->siguiente;
    } 

    anterior->siguiente = crearNodoConstanteEntera(constanteEntera);
}
void imprimirConstantesEnterasDecimales(NodoConstanteEntera *lista) {
    NodoConstanteEntera *actual = lista;
    int suma = 0;
    printf("* Listado de constantes enteras decimales:\n");

    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        suma += atoi(actual->constanteEntera); 
        printf("%s: valor %d\n", actual->constanteEntera, atoi(actual->constanteEntera)); 
        actual = actual->siguiente;
    }
    printf("Total acumulado de sumar todas las constantes  decimales: %d\n", suma);
}

void imprimirConstantesEnterasHexadecimales(NodoConstanteEntera *lista) {
    NodoConstanteEntera *actual = lista;
    printf("* Listado de constantes enteras hexadecimales:\n");

    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("%s: valor entero decimal %d\n", actual->constanteEntera, (int)strtol(actual->constanteEntera, NULL, 16));
        actual = actual->siguiente;
    }
}

void imprimirConstantesEnterasOctales(NodoConstanteEntera *lista) {
    NodoConstanteEntera *actual = lista;
    printf("* Listado de constantes enteras octales:\n");

    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("%s: valor entero decimal %d\n", actual->constanteEntera, (int)strtol(actual->constanteEntera, NULL, 8));
        actual = actual->siguiente;
    }
}

//--

NodoConstanteReal* crearNodoConstanteReal(const char *numero) {
    NodoConstanteReal *nuevoNodoConstanteReal = (NodoConstanteReal *)malloc(sizeof(NodoConstanteReal));
    nuevoNodoConstanteReal->constanteReal = strdup(numero);
    
    nuevoNodoConstanteReal->parteEntera = 0.0;
    nuevoNodoConstanteReal->mantisa = 0.0;
    nuevoNodoConstanteReal->siguiente = NULL;
    
    return nuevoNodoConstanteReal;
}

void agregarConstanteReal(NodoConstanteReal **lista, const char *numero) {
    NodoConstanteReal *nuevo = crearNodoConstanteReal(numero);

    const char *punto = strchr(numero, '.');

    if (punto != NULL) {
        nuevo->parteEntera = atof(strndup(numero, punto - numero));
        nuevo->mantisa = atof(punto); // La mantisa comienza en el punto decimal
    } else {
        nuevo->parteEntera = atof(numero);
        nuevo->mantisa = .0;
    }

    if (*lista == NULL) {
        *lista = nuevo;
    } else {
        NodoConstanteReal *actual = *lista;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
}

void imprimirConstantesReales(NodoConstanteReal *lista) {
    NodoConstanteReal *actual = lista;

    printf("* Listado de constantes reales:\n");

    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("%s: parte entera %.6f, mantisa 0%.6f\n", actual->constanteReal, actual->parteEntera, actual->mantisa);
        actual = actual->siguiente;
    }
}

//--

NodoConstanteCaracter* crearNodoConstanteCaracter(const char *constante) {
    NodoConstanteCaracter *nuevoNodoConstanteCaracter = (NodoConstanteCaracter *)malloc(sizeof(NodoConstanteCaracter));
    nuevoNodoConstanteCaracter->constanteCaracter = strdup(constante);
    nuevoNodoConstanteCaracter->siguiente = NULL;
    return nuevoNodoConstanteCaracter;
}

void agregarConstanteCaracter(NodoConstanteCaracter **lista, const char *constante) {
    NodoConstanteCaracter *nuevo = crearNodoConstanteCaracter(constante);
    
    if (*lista == NULL) {
        *lista = nuevo;
    } else {
        NodoConstanteCaracter *actual = *lista;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
}

void imprimirConstantesCaracteres(NodoConstanteCaracter *lista) {
    NodoConstanteCaracter *actual = lista;
    int indice = 1;
    
    printf("* Listado de constantes caracter enumerados:\n");
    
    while (actual != NULL) {
        printf("%d) %s\n", indice++, actual->constanteCaracter);
        actual = actual->siguiente;
    }
}

// Funciones de Operador/CaracterDePuntuacion:

// Función para crear un nuevo nodo de operador/caracter de puntuación
NodoOperadorYCaracteresDePuntuacion* crearNodoOperadorYCaracteresDePuntuacion(const char *simbolo) {
    NodoOperadorYCaracteresDePuntuacion*newNode = (NodoOperadorYCaracteresDePuntuacion *)malloc(sizeof(NodoOperadorYCaracteresDePuntuacion));
    newNode->simbolo = strdup(simbolo);
    newNode->contador = 1;
    newNode->siguiente = NULL;
    return newNode;
}


// Función para agregar un operador/caracter de puntuación en la lista
void agregarOperadorYCaracteresDePuntuacion(NodoOperadorYCaracteresDePuntuacion **head, const char *simbolo) {
    NodoOperadorYCaracteresDePuntuacion *actual = *head;

    // Buscar si el operador ya existe
    while (actual != NULL) {
        if (strcmp(actual->simbolo, simbolo) == 0) {
            actual->contador++;
            return;
        }
        actual = actual->siguiente;
    }

    // Crear un nuevo nodo
   NodoOperadorYCaracteresDePuntuacion *newNode = crearNodoOperadorYCaracteresDePuntuacion(simbolo);

    // agregar el nuevo nodo al final de la lista
    if (*head == NULL) {
        *head = newNode;
    } else {
       actual= *head;
        while (actual->siguiente != NULL) {
            actual= actual->siguiente;
        }
        actual->siguiente = newNode;
    }
}

// Función para imprimir la lista de operadores/caracteres de puntuación
void imprimirOperadoresYCaracteresDePuntuacion(NodoOperadorYCaracteresDePuntuacion *head) {
    NodoOperadorYCaracteresDePuntuacion *actual = head;
    printf("Listado de operadores/caracteres de puntuación encontrados:\n");
    while (actual!= NULL) {
        printf("%s: aparece %d %s\n", actual->simbolo,actual->contador, actual->contador == 1 ? "vez" : "veces");
        actual = actual->siguiente;
    }
}


//Funciones de Cadena No Reconocida :

// Función para crear un nuevo nodo de cadena no reconocida
NodoCadenaNoReconocida* crearNodoCadenaNoReconocida(const char *CadenaNoReconocida, int linea, int columna) {
    NodoCadenaNoReconocida *newNode = (NodoCadenaNoReconocida *)malloc(sizeof(NodoCadenaNoReconocida));
    newNode->CadenaNoReconocida = strdup(CadenaNoReconocida);
    newNode->linea = linea;
    newNode->columna = columna;
    newNode->siguiente = NULL;
    return newNode;
}


// Función para agregar una cadena no reconocida en la lista
void agregarCadenaNoReconocida(NodoCadenaNoReconocida **head, const char *CadenaNoReconocida, int linea, int columna) {
    NodoCadenaNoReconocida *actual = *head;
    NodoCadenaNoReconocida *anterior = NULL;

    // Crear un nuevo nodo
   NodoCadenaNoReconocida *newNode = crearNodoCadenaNoReconocida(CadenaNoReconocida, linea, columna);

    // agregar el nuevo nodo al final de la lista
    if (actual == NULL) {
        *head = newNode;
    } else {
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = newNode;
    }
}
// Función para imprimir la lista de cadenas no reconocidas
void  imprimirCadenasNoReconocidas(NodoCadenaNoReconocida *head) {
   NodoCadenaNoReconocida *actual = head;
    printf("Listado de cadenas no reconocidas encontradas:\n");
    while (actual != NULL) {
        printf("\"%s\": línea %d, columna %d\n", actual->CadenaNoReconocida, actual->linea, actual->columna);
        actual = actual->siguiente;
    }
}

//--
//Funcion Para Liberar Memoria
/*
void Liberar(lista*head) {
    lista *actual = head;
    while (actual!= NULL) {
       lista *siguiente = actual->siguiente;
        free(actual->valor);
        free(actual);
        actual = siguiente;
    }
}
*/