#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "general.h"

extern YYLTYPE yylloc;
extern int yyval;
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
NodoVariableDeclarada* crearNodoVariableDeclarada(const char *variableDeclarada, const char *tipoDato, const int linea, const char *sufijo){
    NodoVariableDeclarada *nuevo = (NodoVariableDeclarada *)malloc(sizeof(NodoVariableDeclarada));
    nuevo->variableDeclarada = copiarCadena(variableDeclarada);
    nuevo->tipoDato = copiarCadena(tipoDato);
    nuevo->linea = linea;
    nuevo->sufijo = sufijo;
    nuevo->siguiente = NULL;
    return nuevo;
    
}

void agregarVariableDeclarada(NodoVariableDeclarada **lista, const char *variableDeclarada, const char *tipoDato, const int linea, const char *sufijo){
    // Crear el nuevo nodo
    NodoVariableDeclarada *nuevoNodo = crearNodoVariableDeclarada(variableDeclarada, tipoDato, linea, sufijo);

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
    printf("* Listado de variables declaradas (tipo de dato y numero de linea):\n");
    
    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        if(actual->sufijo!=NULL){
            printf("%s: %s %s, linea %d\n", actual->variableDeclarada, actual->sufijo, actual->tipoDato, actual->linea);
        }else{
            printf("%s: %s, linea %d\n", actual->variableDeclarada, actual->tipoDato, actual->linea);
        }
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
        free(actual->sufijo);
        free(actual);
        actual = siguiente;
    }
}

// FUNCIONES
NodoFuncion* crearNodoFuncion(const char *funcion, const char *sufijo, const char *retorno, const int linea, const char* tipogramatica){
    NodoFuncion *nuevo = (NodoFuncion *)malloc(sizeof(NodoFuncion));
    nuevo->funcion = copiarCadena(funcion);
    nuevo->linea = linea;
    nuevo->sufijo = sufijo;
    nuevo->retorno = copiarCadena(retorno);
    nuevo->tipogramatica = copiarCadena(tipogramatica);
    nuevo->siguiente = NULL;

    return nuevo;
}

void agregarFuncion(NodoFuncion **lista, const char *sufijo, const char *retorno, const char *funcion, const int linea, const char* tipogramatica){
    // Crear el nuevo nodo
    NodoFuncion *nuevoNodo = crearNodoFuncion(funcion, sufijo, retorno, linea, tipogramatica);

    // Si la lista está vacía, el nuevo nodo es el primer nodo
    if (*lista == NULL) {
        *lista = nuevoNodo;
        return;
    }

    // Si la lista no está vacía, recorrer hasta el final
    NodoFuncion *actual = *lista;
    while (actual != NULL && actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    // Enlazar el nuevo nodo al final de la lista
    actual->siguiente = nuevoNodo;
}

void imprimirFunciones(NodoFuncion *lista){
    NodoFuncion *actual = lista;
    printf("* Listado de funciones declaradas o definidas:\n");
    
    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        //printf("%s: %s, input: %s, retorna: %s, linea %d\n", actual->funcion, actual->tipogramatica, actual->argumentos, actual->retorno, actual->linea);
        actual = actual->siguiente;
    }
}

void liberarFunciones(NodoFuncion *lista){
    NodoFuncion *actual = lista;
    NodoFuncion *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->funcion);
        free(actual);
        actual = siguiente;
    }
}


// SENTENCIAS
NodoSentencia* crearNodoSentencia(const char *sentencia, const int linea, const int columna){
    NodoSentencia *nuevo = (NodoSentencia *)malloc(sizeof(NodoSentencia));
    nuevo->sentencia = copiarCadena(sentencia);
    nuevo->linea = linea;
    nuevo->columna = columna;
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarSentencia(NodoSentencia **lista, const char *sentencia, const int linea, const int columna) {
    // Crear el nuevo nodo
    NodoSentencia *nuevoNodo = crearNodoSentencia(sentencia, linea, columna);

    // Si la lista está vacía, el nuevo nodo es el primer nodo
    if (*lista == NULL) {
        *lista = nuevoNodo;
        return;
    }

    // Si la lista no está vacía, encontrar la posición correcta para insertar el nuevo nodo
    NodoSentencia *actual = *lista;
    NodoSentencia *anterior = NULL;

    while (actual != NULL && actual->linea < linea) {
        anterior = actual;
        actual = actual->siguiente;
    }

    // Insertar el nuevo nodo en la posición correcta
    if (anterior == NULL) {
        // Insertar al inicio de la lista
        nuevoNodo->siguiente = *lista;
        *lista = nuevoNodo;
    } else {
        // Insertar en medio o al final de la lista
        nuevoNodo->siguiente = actual;
        anterior->siguiente = nuevoNodo;
    }
}

void imprimirSentencias(NodoSentencia *lista){
    NodoSentencia *actual = lista;
    printf("* Listado de sentencias indicando tipo, numero de linea y de columna:\n");
    
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



// ESTRUCTURAS NO RECONOCIDAS
NodoEstructuraNoReconocida* crearNodoEstructuraNoReconocida(const char *estructuraNoReconocida, const int linea){
    NodoEstructuraNoReconocida *nuevo = (NodoEstructuraNoReconocida *)malloc(sizeof(NodoEstructuraNoReconocida));
    nuevo->estructuraNoReconocida = copiarCadena(estructuraNoReconocida);
    nuevo->linea = linea;
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarEstructuraNoReconocida(NodoEstructuraNoReconocida **lista, const char *estructuraNoReconocida, const int linea){
    // Crear el nuevo nodo
    NodoEstructuraNoReconocida *nuevoNodo = crearNodoEstructuraNoReconocida(estructuraNoReconocida,linea);
    
    if (*lista == NULL) {
        *lista = nuevoNodo;
        return;

    // Si la lista no está vacía, recorrer hasta el final
    NodoEstructuraNoReconocida *actual = *lista;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    // Enlazar el nuevo nodo al final de la lista
    actual->siguiente = nuevoNodo;    
    }
}

void imprimirEstructurasNoReconocidas(NodoEstructuraNoReconocida *lista){
    NodoEstructuraNoReconocida *actual = lista;
    printf("* Listado de estructuras sintácticas no reconocidas\n");
    
    if (actual == NULL) {
       // printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("%s: linea %d\n", actual->estructuraNoReconocida, actual->linea);
        actual = actual->siguiente;
    }
}

void liberarEstructurasNoReconocidas(NodoEstructuraNoReconocida *lista){
    NodoEstructuraNoReconocida *actual = lista;
    NodoEstructuraNoReconocida *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->estructuraNoReconocida);
        free(actual);
        actual = siguiente;
    }
}



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


// Funciones de Utilidad
char* copiarCadena(const char *str) {
    size_t len = strlen(str);  // Obtiene la longitud de la cadena de entrada
    char *copiado = (char *)malloc(len + 1);  // Asigna memoria para la nueva cadena
    if (copiado != NULL) {
        strcpy(copiado, str);  // Copia el contenido de la cadena de entrada a la nueva cadena
    }
    return copiado;  // Devuelve el puntero a la nueva cadena copiada
}