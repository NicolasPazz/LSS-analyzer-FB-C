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

NodoVariableDeclarada* crearNodoVariableDeclarada(const char *variableDeclarada){}

void agregarVariableDeclarada(NodoVariableDeclarada **lista, const char *variableDeclarada, const char *tipoDato){}

void imprimirVariablesDeclaradas(NodoVariableDeclarada *lista){}

void liberarVariablesDeclaradas(NodoVariableDeclarada *lista){}

NodoVariableDeclarada* listaVariablesDeclaradas = NULL;


// FUNCIONES

NodoFuncion* crearNodoFuncion(const char *funcion){}

void agregarFuncion(NodoFuncion **lista, const char *funcion){}

void imprimirFunciones(NodoFuncion *lista){}

void liberarFunciones(NodoFuncion *lista){}

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
NodoEstructuraNoReconocida* crearNodoEstructuraNoReconocida(const char *estructuraNoReconocida){

    NodoEstructuraNoReconocida *nuevo = (NodoEstructuraNoReconocida *)malloc(sizeof(NodoEstructuraNoReconocida));
    nuevo->estructuraNoReconocida = copiarCadena(estructuraNoReconocida);
    //nuevo->linea = linea;
    nuevo->siguiente = NULL;

    return nuevo;

}

void agregarEstructuraNoReconocida(NodoEstructuraNoReconocida **lista, const char *estructuraNoReconocida){

    // Crear el nuevo nodo
    NodoEstructuraNoReconocida *nuevoNodo = crearNodoEstructuraNoReconocida(estructuraNoReconocida);

    
    if (*lista == NULL) {
        *lista = nuevoNodo;
        return;
    }

    // Si la lista no está vacía, recorrer hasta el final
    NodoEstructuraNoReconocida *actual = *lista;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    // Enlazar el nuevo nodo al final de la lista
    actual->siguiente = nuevoNodo;

}

void imprimirEstructurasNoReconocidas(NodoEstructuraNoReconocida *lista){

    /*NodoEstructuraNoReconocida *actual = lista;
    printf("* Listado de estructuras no reconocidas:\n");
    
    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("%s: linea %d\n", actual->estructuraNoReconocida, actual->linea);
        actual = actual->siguiente;
    }*/

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


// FUNCIONES

char* copiarCadena(const char *str) {
    size_t len = strlen(str);  // Obtiene la longitud de la cadena de entrada
    char *copiado = (char *)malloc(len + 1);  // Asigna memoria para la nueva cadena
    if (copiado != NULL) {
        strcpy(copiado, str);  // Copia el contenido de la cadena de entrada a la nueva cadena
    }
    return copiado;  // Devuelve el puntero a la nueva cadena copiada
}