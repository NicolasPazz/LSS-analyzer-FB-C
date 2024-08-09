#include "funciones.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// IDENTIFICADORES

NodoIdentificador* crearNodoIdentificador(const char *identificador) {
    NodoIdentificador *nuevoNodoIdentificador = (NodoIdentificador *)malloc(sizeof(NodoIdentificador)); 
    nuevoNodoIdentificador->identificador = strdup(identificador);
    nuevoNodoIdentificador->contador = 1;
    nuevoNodoIdentificador->siguiente = NULL;
    return nuevoNodoIdentificador;
}

void agregarIdentificador(NodoIdentificador **lista, const char *identificador) {
    NodoIdentificador *actual = *lista;
    NodoIdentificador *anterior = NULL;

    //iterar sobre la lista hasta que encuentre el identificador o llegue al final
    while (actual != NULL) {
        if (strcmp(actual->identificador, identificador) == 0) {
            actual->contador++;
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }

    //Si no lo encuentra en la lista lo agrega ordenado
    NodoIdentificador *nuevoNodo = crearNodoIdentificador(identificador);

    if (*lista == NULL || strcmp((*lista)->identificador, identificador) > 0) {
        nuevoNodo->siguiente = *lista;
        *lista = nuevoNodo;
    } else {
        actual = *lista;
        while (actual->siguiente != NULL && strcmp(actual->siguiente->identificador, identificador) < 0) {
            actual = actual->siguiente;
        }
        nuevoNodo->siguiente = actual->siguiente;
        actual->siguiente = nuevoNodo;
    }
}

void imprimirIdentificadores(NodoIdentificador *lista) {
    NodoIdentificador *actual = lista;
    printf("* Listado de identificadores encontrados:\n");

    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("%s: aparece %d %s\n", actual->identificador, actual->contador, actual->contador == 1 ? "vez" : "veces");
        actual = actual->siguiente;
    }
}

void liberarIdentificadores(NodoIdentificador *lista) {
    NodoIdentificador *actual = lista;
    NodoIdentificador *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->identificador);
        free(actual);
        actual = siguiente;
    }
}


// LITERALES CADENA

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

void imprimirLiteralesCadena(NodoLiteralCadena *lista) {
    NodoLiteralCadena *actual = listaLiteralesCadena;
    printf("* Listado de literales cadena encontrados:\n");

    if (actual == NULL) {
        printf("-\n");
        return;
    }
    
    while (actual != NULL) {
        printf("\"%s\": longitud %d\n", actual->literalCadena, actual->longitud);
        actual = actual->siguiente;
    }
}

void liberarLiteralesCadena(NodoLiteralCadena *lista) {
    NodoLiteralCadena *actual = lista;
    NodoLiteralCadena *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->literalCadena);
        free(actual);
        actual = siguiente;
    }
}

// PALABRAS RESERVADAS

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

void imprimirPalabrasReservadas(NodoPalabraReservada *listaPalabrasReservadasPorTipoDeDato, NodoPalabraReservada *listaPalabrasReservadasPorEstructuraDeControl, NodoPalabraReservada *listaOtrasPalabrasReservadas){
    NodoPalabraReservada *actual = listaPalabrasReservadasPorTipoDeDato;
    printf("* Listado de palabras reservadas (tipos de dato):\n");

    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("%s: linea %d, columna %d\n", actual->palabraReservada, actual->linea, actual->columna);
        actual = actual->siguiente;
    }

    actual = listaPalabrasReservadasPorEstructuraDeControl;
    printf("* Listado de palabras reservadas (estructuras de control):\n");

    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("%s: linea %d, columna %d\n", actual->palabraReservada, actual->linea, actual->columna);
        actual = actual->siguiente;
    }

    actual = listaOtrasPalabrasReservadas;
    printf("* Listado de palabras reservadas  (otros):\n");

    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("%s: linea %d, columna %d\n", actual->palabraReservada, actual->linea, actual->columna);
        actual = actual->siguiente;
    }
}

void liberarPalabrasReservadas(NodoPalabraReservada *lista) {
    NodoPalabraReservada *actual = lista;
    NodoPalabraReservada *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->palabraReservada);
        free(actual);
        actual = siguiente;
    }
}

// CONSTANTES ENTERAS

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

void liberarConstantesEnteras(NodoConstanteEntera *lista) {
    NodoConstanteEntera *actual = lista;
    NodoConstanteEntera *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->constanteEntera);
        free(actual);
        actual = siguiente;
    }
}

// CONSTANTES REALES

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

void liberarConstantesReales(NodoConstanteReal *lista) {
    NodoConstanteReal *actual = lista;
    NodoConstanteReal *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->constanteReal);
        free(actual);
        actual = siguiente;
    }
}

// CONSTANTES CARACTERES

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
    
    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("%d) %s\n", indice++, actual->constanteCaracter);
        actual = actual->siguiente;
    }
}

void liberarConstantesCaracteres(NodoConstanteCaracter *lista) {
    NodoConstanteCaracter *actual = lista;
    NodoConstanteCaracter *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->constanteCaracter);
        free(actual);
        actual = siguiente;
    }
}

// OPERADORES Y CARACTERES DE PUNTUACION

NodoOperadorYCaracteresDePuntuacion* crearNodoOperadorYCaracteresDePuntuacion(const char *simbolo) {
    NodoOperadorYCaracteresDePuntuacion *nuevo = (NodoOperadorYCaracteresDePuntuacion *)malloc(sizeof(NodoOperadorYCaracteresDePuntuacion));
    nuevo->simbolo = strdup(simbolo);
    nuevo->contador = 1;
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarOperadorYCaracteresDePuntuacion(NodoOperadorYCaracteresDePuntuacion **lista, const char *simbolo) {
    NodoOperadorYCaracteresDePuntuacion *actual = *lista;
    NodoOperadorYCaracteresDePuntuacion *anterior = NULL;

    while (actual != NULL) {
        //Si lo encuentra en la lista aumenta el contador
        if (strcmp(actual->simbolo, simbolo) == 0) {
            actual->contador++;
            return;
        }
        anterior = actual;
        actual = actual->siguiente;
    }

    //Si no lo encuentra en la lista lo agrega al final
    NodoOperadorYCaracteresDePuntuacion *nuevo = crearNodoOperadorYCaracteresDePuntuacion(simbolo);

    if (*lista == NULL) {
        *lista = nuevo;
    } else {
        actual = *lista;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
}

void imprimirOperadoresYCaracteresDePuntuacion(NodoOperadorYCaracteresDePuntuacion *lista) {
    NodoOperadorYCaracteresDePuntuacion *actual = lista;
    printf("Listado de operadores/caracteres de puntuación :\n");

    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual!= NULL) {
        printf("%s: aparece %d %s\n", actual->simbolo,actual->contador, actual->contador == 1 ? "vez" : "veces");
        actual = actual->siguiente;
    }
}

void liberarOperadoresYCaracteresDePuntuacion(NodoOperadorYCaracteresDePuntuacion *lista) {
    NodoOperadorYCaracteresDePuntuacion *actual = lista;
    NodoOperadorYCaracteresDePuntuacion *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->simbolo);
        free(actual);
        actual = siguiente;
    }
}

// CADENAS NO RECONOCIDAS

NodoCadenaNoReconocida* crearNodoCadenaNoReconocida(const char *cadenaNoReconocida, int linea, int columna) {
    NodoCadenaNoReconocida *nuevo = (NodoCadenaNoReconocida *)malloc(sizeof(NodoCadenaNoReconocida));
    nuevo->cadenaNoReconocida = strdup(CadenaNoReconocida);
    nuevo->linea = linea;
    nuevo->columna = columna;
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarCadenaNoReconocida(NodoCadenaNoReconocida **lista, const char *cadenaNoReconocida, int linea, int columna){
    NodoCadenaNoReconocida *actual = *lista;
    NodoCadenaNoReconocida *anterior = NULL;

    while (actual != NULL) {
        anterior = actual; 
        actual = actual->siguiente;
    } 
    
    anterior->siguiente = crearNodoCadenaNoReconocida(cadenaNoReconocida, linea, columna); 
}

void  imprimirCadenasNoReconocidas(NodoCadenaNoReconocida *lista) {
   NodoCadenaNoReconocida *actual = lista;
    printf("Listado de cadenas no reconocidas:\n");
    while (actual != NULL) {
        printf("%s: linea %d, columna %d\n", actual->cadenaNoReconocida, actual->linea, actual->columna);
        actual = actual->siguiente;
    }
}

void liberarCadenasNoReconocidas(NodoCadenaNoReconocida *lista) {
    NodoCadenaNoReconocida *actual = lista;
    NodoCadenaNoReconocida *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->cadenaNoReconocida);
        free(actual);
        actual = siguiente;
    }
}