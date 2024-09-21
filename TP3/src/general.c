/* En los archivos (*.c) se pueden poner tanto DECLARACIONES como DEFINICIONES de C, así como directivas de preprocesador */
/* Recordar solamente indicar archivos *.h en las directivas de preprocesador #include, nunca archivos *.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "general.h"

YYLTYPE yylloc;

void pausa(void)
{
    printf("Presione ENTER para continuar...\n");
    getchar();
}

void inicializarUbicacion(void)
{
    yylloc.first_line = yylloc.last_line = INICIO_CONTEO_LINEA;
    yylloc.first_column = yylloc.last_column = INICIO_CONTEO_COLUMNA;
}

void reinicializarUbicacion(void)
{
    yylloc.first_line = yylloc.last_line;
    yylloc.first_column = yylloc.last_column;
}


// VARIABLES DECLARADAS
/*
NodoVariableDeclarada* listaVariablesDeclaradas = NULL;


NodoVariableDeclarada* crearNodoVariableDeclarada(const char *variableDeclarada, const char *tipoDato, const int linea){
    NodoVariableDeclarada *nuevo = (NodoVariableDeclarada *)malloc(sizeof(NodoVariableDeclarada));
    nuevo->variableDeclarada = copiarCadena(variableDeclarada);
    nuevo->linea = linea;
    nuevo->tipoDato = copiarCadena(tipoDato);
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarVariableDeclarada(NodoVariableDeclarada **lista, const char *variableDeclarada, const char *tipoDato, const int linea) {
    // Crear el nuevo nodo
    NodoVariableDeclarada *nuevoNodo = crearNodoVariableDeclarada(variableDeclarada, tipoDato, linea);

    // Si la lista está vacía, el nuevo nodo es el primer nodo
    if (*lista == NULL) {
        *lista = nuevoNodo;
        return;
    }

    // Si la lista no está vacía, recorrer hasta el final
    NodoVariableDeclarada *actual = *lista;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    // Enlazar el nuevo nodo al final de la lista
    actual->siguiente = nuevoNodo;
}

void imprimirVariablesDeclaradas(NodoVariableDeclarada *lista){
    NodoVariableDeclarada *actual = lista;
    printf("* Listado de variables declaradas:\n");
    
    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("%s: %s, linea %d\n", actual->variableDeclarada, actual->tipoDato, actual->linea);
        actual = actual->siguiente;
    }
}

void liberarVariablesDeclaradas(NodoVariableDeclarada *lista){
    NodoVariableDeclarada *actual = lista;
    NodoVariableDeclarada *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->variableDeclarada);
        free(actual->tipoDato);
        free(actual);
        actual = siguiente;
    }
}
*/


// FUNCIONES

NodoFuncion* crearNodoFuncion(const char *funcion){};

void agregarFuncion(NodoFuncion **lista, const char *funcion){};

void imprimirFunciones(NodoFuncion *lista){};

void liberarFunciones(NodoFuncion *lista){};

NodoFuncion* listaFunciones = NULL;


// SENTENCIAS

NodoSentencia* crearNodoSentencia(const char *sentencia, const int linea, const int columna){
    NodoSentencia *nuevo = (NodoSentencia *)malloc(sizeof(NodoSentencia));
    nuevo->sentencia = copiarCadena(sentencia);
    nuevo->linea = linea;
    nuevo->columna = columna;
    nuevo->siguiente = NULL;
    return nuevo;
}


void agregarSentencia(NodoSentencia **lista, const char *sentencia, const int linea, const int columna){
     // Crear el nuevo nodo
    NodoSentencia *nuevoNodo = crearNodoSentencia(sentencia, linea, columna);

    // Si la lista está vacía, el nuevo nodo es el primer nodo
    if (*lista == NULL) {
        *lista = nuevoNodo;
        return;
    }

    // Si la lista no está vacía, recorrer hasta el final
    NodoSentencia *actual = *lista;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    // Enlazar el nuevo nodo al final de la lista
    actual->siguiente = nuevoNodo;
}

void imprimirSentencias(NodoSentencia *lista){
    NodoSentencia *actual = lista;
    printf("* Listado de sentencias:\n");
    
    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("%s: linea %d, columna %d\n", actual->sentencia, actual->linea, actual->columna);
        actual = actual->siguiente;
    }
}

void liberarSentencias(NodoSentencia *lista){
    NodoSentencia *actual = lista;
    NodoSentencia *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->sentencia);
        free(actual);
        actual = siguiente;
    }
}

NodoSentencia* listaSentencias = NULL;


// ESTRUCTURAS NO RECONOCIDAS

NodoEstructuraNoReconocida* crearNodoEstructuraNoReconocida(const char *estructuraNoReconocida){};

void agregarEstructuraNoReconocida(NodoEstructuraNoReconocida **lista, const char *estructuraNoReconocida){};

void imprimirEstructurasNoReconocidas(NodoEstructuraNoReconocida *lista){};

void liberarEstructurasNoReconocidas(NodoEstructuraNoReconocida *lista){};

NodoEstructuraNoReconocida* listaEstructurasNoReconocidas = NULL;


// CADENAS NO RECONOCIDAS

NodoCadenaNoReconocida* crearNodoCadenaNoReconocida(const char *cadenaNoReconocida, int linea, int columna) {
    NodoCadenaNoReconocida *nuevo = (NodoCadenaNoReconocida *)malloc(sizeof(NodoCadenaNoReconocida));
    nuevo->cadenaNoReconocida = copiarCadena(cadenaNoReconocida);
    nuevo->linea = linea;
    nuevo->columna = columna;
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarCadenaNoReconocida(NodoCadenaNoReconocida **lista, const char *cadenaNoReconocida, int linea, int columna){
    // Crear el nuevo nodo
    NodoCadenaNoReconocida *nuevoNodo = crearNodoCadenaNoReconocida(cadenaNoReconocida, linea, columna);

    // Si la lista está vacía, el nuevo nodo es el primer nodo
    if (*lista == NULL) {
        *lista = nuevoNodo;
        return;
    }

    // Si la lista no está vacía, recorrer hasta el final
    NodoCadenaNoReconocida *actual = *lista;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    // Enlazar el nuevo nodo al final de la lista
    actual->siguiente = nuevoNodo;
}

void  imprimirCadenasNoReconocidas(NodoCadenaNoReconocida *lista) {
   NodoCadenaNoReconocida *actual = lista;
    printf("* Listado de cadenas no reconocidas:\n");
    
    if (actual == NULL) {
        printf("-\n");
        return;
    }

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

NodoCadenaNoReconocida* listaCadenasNoReconocidas  = NULL;

// DEFINICIONES EXTERNAS

NodoFuncionExterna* crearNodoFuncionExterna(const char *nombre, const char *tipoRetorno){};

void agregarParametro(NodoFuncionExterna *funcion, const char *tipo, const char *nombre){};

void agregarFuncion(NodoFuncionExterna **lista, const char *nombre, const char *tipoRetorno){};

void imprimirListaDeFuncionesExterna(NodoFuncionExterna *lista){};

void liberarListaDeFunciones(NodoFuncionExterna *lista){};

//CREAR NODO DE FUNCION
NodoFuncionExterna* crearNodoFuncionExterna(const char *nombre, const char *tipoRetorno) {
    NodoFuncionExterna *nuevo = (NodoFuncionExterna *)malloc(sizeof(NodoFuncionExterna));
    nuevo->nombre = copiarCadena(nombre);
    nuevo->tipoRetorno = copiarCadena(tipoRetorno);
    nuevo->parametros = NULL;
    nuevo->siguiente = NULL;
    return nuevo;
}

// AGREGAR PARAMETRO EN LA FUNCION
void agregarParametro(NodoFuncionExterna *funcion, const char *tipo, const char *nombre) {
    NodoParametro *nuevoParametro = (NodoParametro *)malloc(sizeof(NodoParametro));
    nuevoParametro->tipo = copiarCadena(tipo);
    nuevoParametro->nombre = copiarCadena(nombre);
    nuevoParametro->siguiente = NULL;

    // Si no hay parámetros, este es el primero
    if (funcion->parametros == NULL) {
        funcion->parametros = nuevoParametro;
    } else {
        // Si ya hay parámetros, lo agregamos al final
        NodoParametro *actual = funcion->parametros;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoParametro;
    }
}

// AGREGAR FUNCION EN UNA LISTA DE FUNCIONES 
void agregarFuncionExterna(NodoFuncionExterna **lista, const char *nombre, const char *tipoRetorno) {
    NodoFuncionExterna *nuevaFuncionExterna = crearNodoFuncionExterna(nombre, tipoRetorno);

    // Si la lista está vacía, esta es la primera función
    if (*lista == NULL) {
        *lista = nuevaFuncionExterna;
    } else {
        // Si ya hay funciones, la agregamos al final
        NodoFuncionExterna *actual = *lista;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevaFuncionExterna;
    }
}

// IMPRIMIR FUNCIONES
void imprimirListaDeFuncionesExterna(NodoFuncionExterna *lista) {
    NodoFuncionExterna *actual = lista;
    printf("* Listado de funciones declaradas/definidas:\n");
    
    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("Función: %s\n", actual->nombre);
        printf("Tipo de retorno: %s\n", actual->tipoRetorno);
        printf("Parámetros:\n");

        NodoParametro *parametroActual = actual->parametros;
        if (parametroActual == NULL) {
            printf("Sin parámetros\n");
        } else {
            while (parametroActual != NULL) {
                printf("  %s %s\n", parametroActual->tipo, parametroActual->nombre);
                parametroActual = parametroActual->siguiente;
            }
        }
        actual = actual->siguiente;
        printf("\n");
    }
}

// LIBERAR FUNCIONES 
void liberarListaDeFunciones(NodoFuncionExterna *lista) {
    NodoFuncionExterna *actual = lista;
    NodoFuncionExterna *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;

        // Liberar los parámetros
        NodoParametro *parametroActual = actual->parametros;
        NodoParametro *paramSiguiente = NULL;
        while (parametroActual != NULL) {
            paramSiguiente = parametro->siguiente;
            free(parametroActual->tipo);
            free(parametroActual->nombre);
            free(parametroActual);
            parametro = paramSiguiente;
        }

        // Liberar la función
        free(actual->nombre);
        free(actual->tipoRetorno);
        free(actual);
        actual = siguiente;
    }
}



// FUNCIONES

char* copiarCadena(const char *str) {
    size_t len = strlen(str);  // Obtiene la longitud de la cadena de entrada
    char *copiado = (char *)malloc(len + 1);  // Asigna memoria para la nueva cadena
    if (copiado != NULL) {
        strcpy(copiado, str);  // Copia el contenido de la cadena de entrada a la nueva cadena
    }
    return copiado;  // Devuelve el puntero a la nueva cadena copiada
}