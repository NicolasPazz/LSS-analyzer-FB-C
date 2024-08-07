#include "funciones.h"

// Función para crear un nuevo NodoIdentificador
NodoIdentificador* crearNodoIdentificador(const char *identificador) {
    NodoIdentificador *nuevoNodoIdentificador = (NodoIdentificador *)malloc(sizeof(NodoIdentificador)); 
    nuevoNodoIdentificador->identificador = strdup(identificador);
    nuevoNodoIdentificador->contador = 1;
    nuevoNodoIdentificador->siguiente = NULL;
    return nuevoNodoIdentificador;
}

// Función para agregar una identificador a la lista
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

// Función para imprimir la lista
void imprimirListaIdentificador(NodoIdentificador *lista) {
    NodoIdentificador *actual = lista;
    while (actual != NULL) {
        printf("%s: %d\n", actual->identificador, actual->contador);
        actual = actual->siguiente;
    }
}

// Función para liberar la memoria de la lista
void liberarListaIdentificador(NodoIdentificador *lista) {
    NodoIdentificador *actual = lista;
    while (actual != NULL) {
        NodoIdentificador *temp = actual;
        actual = actual->siguiente;
        free(temp->identificador);
        free(temp);
    }
}

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

void imprimirListaLiteralCadena(NodoLiteralCadena *lista) {
    NodoLiteralCadena *actual = lista;
    while (actual != NULL) {
        printf("%s: %d\n", actual->literalCadena, actual->longitud);
        actual = actual->siguiente;
    }
}

void liberarListaLiteralCadena(NodoLiteralCadena *lista) {
    NodoLiteralCadena *actual = lista;
    while (actual != NULL) {
        NodoLiteralCadena *temp = actual;
        actual = actual->siguiente;
        free(temp->literalCadena);
        free(temp);
    }
}
