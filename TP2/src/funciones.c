#include "funciones.h"

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

NodoConstanteEntera* crearNodoConstanteEntera(int *constanteEntera) {
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
        printf("- \n");
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
        printf("- \n");
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
        printf("- \n");
        return;
    }
    
    while (actual != NULL) {
        printf("%s: valor entero decimal %d\n", actual->constanteEntera, (int)strtol(actual->constanteEntera, NULL, 8));
        actual = actual->siguiente;
    }
}

//--

