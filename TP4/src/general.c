#include "general.h"

extern YYLTYPE yylloc;
extern int yyval;

void pausa(void){
    printf("Presione ENTER para continuar...\n");
    getchar();
}

void inicializarUbicacion(void){
    yylloc.first_line = yylloc.last_line = INICIO_CONTEO_LINEA;
    yylloc.first_column = yylloc.last_column = INICIO_CONTEO_COLUMNA;
}

void reinicializarUbicacion(void){
    yylloc.first_line = yylloc.last_line;
    yylloc.first_column = yylloc.last_column;
}


// ESTRUCTURAS DE DATOS
const char *tipoFuncionString[] = {
    [DEFINICION_FUNCION] = "definicion",
    [DECLARACION_FUNCION] = "declaracion",
    [OTRO] = "ERROR"
};

const char *especificadorTiposString[] = {
    [VACIO_TIPODATO] = "vacio",
    [CHAR_TIPODATO] = "char",
    [VOID_TIPODATO] = "void",
    [DOUBLE_TIPODATO] = "double",
    [FLOAT_TIPODATO] = "float",
    [INT_TIPODATO] = "int",
    [UNSIGNED_INT_TIPODATO] = "unsigned int",
    [LONG_TIPODATO] = "long",
    [UNSIGNED_LONG_TIPODATO] = "unsigned long",
    [SHORT_TIPODATO] = "short",
    [UNSIGNED_SHORT_TIPODATO] = "unsigned short"
};

const char *especificadorAlmacenamientoString[] = {
    [AUTO_ESPALMAC] = "auto",
    [REGISTER_ESPALMAC] = "register",
    [STATIC_ESPALMAC] = "static",
    [EXTERN_ESPALMAC] = "extern",
    [TYPEDEF_ESPALMAC] = "typedef",
    [VACIO_ESPALMAC] = "vacio"
};

const char *calificadorTipoString[] = {
    [CONST_CALIFICADORTIPO] = "const",
    [VOLATILE_CALIFICADORTIPO] = "volatile",
    [VACIO_CALIFICADORTIPO] = "vacio"
};

EspecificadorTipos combinarEspecificadorTipos(EspecificadorTipos a, EspecificadorTipos b){
    EspecificadorTipos inicial = (struct EspecificadorTipos){.esTipoDato = VACIO_TIPODATO, .esAlmacenamiento = VACIO_ESPALMAC, .esCalificador = VACIO_CALIFICADORTIPO};
    if (a.esTipoDato != VACIO_TIPODATO)
        inicial.esTipoDato = a.esTipoDato;
    if (a.esAlmacenamiento != VACIO_ESPALMAC)
        inicial.esAlmacenamiento = a.esAlmacenamiento;
    if (a.esCalificador != VACIO_CALIFICADORTIPO)
        inicial.esCalificador = a.esCalificador;

    if (b.esTipoDato != VACIO_TIPODATO)
        inicial.esTipoDato = b.esTipoDato;
    if (b.esAlmacenamiento != VACIO_ESPALMAC)
        inicial.esAlmacenamiento = b.esAlmacenamiento;
    if (b.esCalificador != VACIO_CALIFICADORTIPO)
        inicial.esCalificador = b.esCalificador;

    return inicial;
}

// TABLA DE SIMBOLOS
NodoSimbolo *crearNodoSimbolo(const char *nombre, tipoSimbolo tipo, int linea, int columna, void *nodo){
    // Asignar memoria para el nuevo nodo de tipo NodoSimbolo
    NodoSimbolo *nuevo = (NodoSimbolo *)malloc(sizeof(NodoSimbolo));
    if (nuevo == NULL){
        printf("Error: no se pudo asignar memoria para el nuevo nodo de simbolo.\n");
        return NULL;
    }

    // Asignar los valores a los campos del NodoSimbolo
    nuevo->nombre = copiarCadena(nombre); // Copiar el nombre del simbolo
    nuevo->tipo = tipo;                   // Asignar el tipo del simbolo (VARIABLE o FUNCION)
    nuevo->nodo = nodo;                   // Asignar el nodo asociado (de tipo void*)
    nuevo->linea = linea;                 // Asignar la linea actual leida (donde comienza la funcion)
    nuevo->columna = columna;             // Asignar la columna actual leida (del identificador)
    nuevo->siguiente = NULL;              // Inicializar el siguiente puntero a NULL

    return nuevo; // Retornar el nuevo nodo
}

NodoSimbolo *buscar_simbolo(char *nombre){
    NodoSimbolo *nodo_actual = tablaSimbolos;

    while (nodo_actual != NULL){
        if (strcmp(nodo_actual->nombre, nombre) == 0){
            return nodo_actual; // Retornar el nodo encontrado
        }
        nodo_actual = nodo_actual->siguiente; // Mover al siguiente nodo
    }
    return NULL; // Retornar NULL si no se encuentra el nodo
}

void imprimirTablaSimbolos(NodoSimbolo *tablaSimbolos){
    NodoSimbolo *nodoActual = tablaSimbolos;

    printf("Tabla de Simbolos:\n");
    if (nodoActual == NULL){
        printf("La tabla de simbolos esta vacia :( \n\n");
    }

    while (nodoActual != NULL){
        printf("Nombre: %s\n", nodoActual->nombre);
        printf("Tipo: %s\n", nodoActual->tipo == VARIABLE ? "VARIABLE" : "FUNCION");
        printf("Linea: %d\n", nodoActual->linea);
        printf("Columna: %d\n", nodoActual->columna);

        if (nodoActual->tipo == VARIABLE){
            NodoVariableDeclarada *var = (NodoVariableDeclarada *)nodoActual->nodo;
            printf("Tipo de dato: %s, Almacenamiento: %s, Calificador: %s\n", especificadorTiposString[var->tipoDato.esTipoDato], especificadorAlmacenamientoString[var->tipoDato.esAlmacenamiento], calificadorTipoString[var->tipoDato.esCalificador]);
        }
        else if (nodoActual->tipo == FUNCION){
            NodoFuncion *func = (NodoFuncion *)nodoActual->nodo;
            printf("Retorno: Tipo de dato: %s, Almacenamiento: %s, Calificador: %s\n", especificadorTiposString[func->retorno.esTipoDato], especificadorAlmacenamientoString[func->retorno.esAlmacenamiento], calificadorTipoString[func->retorno.esCalificador]);
            printf("Tipo de gramatica: %s\n", func->tipogramatica == DEFINICION_FUNCION ? "DEFINICION" : "DECLARACION");

            char *parametros = imprimirParametros(func->listaDeParametros);
            printf("  Parametros: %s\n\n", parametros);
            free(parametros); // Liberar memoria despues de imprimir
        }
        printf("\n");
        nodoActual = nodoActual->siguiente;
    }
}


// CADENAS NO RECONOCIDAS - ERRORES LEXICOS
NodoCadenaNoReconocida *crearNodoCadenaNoReconocida(const char *cadenaNoReconocida, int linea, int columna){
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

    // Si la lista esta vacia, el nuevo nodo es el primer nodo
    if (*lista == NULL){
        *lista = nuevoNodo;
        return;
    }

    // Si la lista no esta vacia, recorrer hasta el final
    NodoCadenaNoReconocida *actual = *lista;
    while (actual->siguiente != NULL){
        actual = actual->siguiente;
    }

    // Enlazar el nuevo nodo al final de la lista
    actual->siguiente = nuevoNodo;
}

void imprimirCadenasNoReconocidas(NodoCadenaNoReconocida *lista){
    NodoCadenaNoReconocida *actual = lista;
    printf("* Listado de errores lexicos:\n");

    if (actual == NULL){
        printf("-\n");
        return;
    }

    while (actual != NULL){
        printf("%s: linea %d, columna %d\n", actual->cadenaNoReconocida, actual->linea, actual->columna);
        actual = actual->siguiente;
    }
}

void liberarCadenasNoReconocidas(NodoCadenaNoReconocida *lista){
    NodoCadenaNoReconocida *actual = lista;
    NodoCadenaNoReconocida *siguiente = NULL;

    while (actual != NULL){
        siguiente = actual->siguiente;
        free(actual->cadenaNoReconocida);
        free(actual);
        actual = siguiente;
    }
}


// ESTRUCTURAS NO RECONOCIDAS - ERRORES SINTACTICOS
NodoErrorSintactico *crearNodoErrorSintactico(const char *errorSintactico, const int linea){
    NodoErrorSintactico *nuevo = (NodoErrorSintactico *)malloc(sizeof(NodoErrorSintactico));
    nuevo->errorSintactico = copiarCadena(errorSintactico);
    nuevo->linea = linea;
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarErrorSintactico(NodoErrorSintactico **listaErroresSintacticos, NodoErrorSintactico **listaSecuenciasLeidas){
    if (*listaSecuenciasLeidas == NULL)
        return; // No hay nada en la lista de secuencias leidas

    NodoErrorSintactico *nodoError = NULL;
    NodoErrorSintactico *actual = *listaSecuenciasLeidas;

    // Verificar si hay solo un nodo
    if (actual->siguiente == NULL){
        // Si solo hay un nodo, copiar ese nodo
        nodoError = crearNodoErrorSintactico(actual->errorSintactico, actual->linea);
    }
    else{
        // Recorrer hasta el anteultimo nodo
        while (actual->siguiente != NULL && actual->siguiente->siguiente != NULL && actual->siguiente->siguiente->siguiente != NULL){
            actual = actual->siguiente;
        }
        // Ahora `actual` es el anteultimo nodo
        eliminarEspacios(actual->errorSintactico);
        nodoError = crearNodoErrorSintactico(actual->errorSintactico, actual->linea); // Copiar el anteultimo nodo
    }

    // Agregar el nodo seleccionado a la lista de errores sintacticos
    nodoError->siguiente = *listaErroresSintacticos;
    *listaErroresSintacticos = nodoError;
}

void eliminarEspacios(char *cadena){
    char *src = cadena; // Puntero para recorrer la cadena
    char *dst = cadena; // Puntero para la posicion de escritura

    // Mover el puntero src hasta el primer caracter no espacio
    while (*src == ' '){
        src++;
    }

    // Copiar el resto de la cadena, omitiendo los espacios al inicio
    while (*src){
        *dst++ = *src++;
    }
    *dst = '\0'; // Terminar la cadena resultante
}

void imprimirErrorSintactico(NodoErrorSintactico *lista){
    NodoErrorSintactico *actual = lista;
    printf("* Listado de errores sintacticos:\n");

    if (actual == NULL){
        printf("-\n");
        return;
    }

    while (actual != NULL){
        printf("\"%s\": linea %d\n", actual->errorSintactico, actual->linea);
        actual = actual->siguiente;
    }
}

void liberarErrorSintactico(NodoErrorSintactico *lista){
    NodoErrorSintactico *actual = lista;
    NodoErrorSintactico *siguiente = NULL;

    while (actual != NULL){
        siguiente = actual->siguiente;
        free(actual->errorSintactico);
        free(actual);
        actual = siguiente;
    }
}

void concatenarLeido(NodoErrorSintactico **listaSecuenciasLeidas, const char *yytext, int linea){
    // Si es un caracter de corte, crea un nuevo nodo y lo agrega al final de la lista
    if (strcmp(yytext, ";") == 0 || strcmp(yytext, "\n") == 0){
        if (*listaSecuenciasLeidas == NULL){
            // Si la lista esta vacia, crea el primer nodo
            *listaSecuenciasLeidas = crearNodoErrorSintactico("", linea);
        }
        else{
            // Si hay nodos, busca el final de la lista
            NodoErrorSintactico *nuevoNodo = crearNodoErrorSintactico("", linea);
            NodoErrorSintactico *actual = *listaSecuenciasLeidas;
            // Recorrer hasta el ultimo nodo
            while (actual->siguiente != NULL){
                actual = actual->siguiente;
            }
            // Enlazar el nuevo nodo al final
            actual->siguiente = nuevoNodo;
        }
    }
    else{
        // Si la lista esta vacia, crea el primer nodo
        if (*listaSecuenciasLeidas == NULL){
            *listaSecuenciasLeidas = crearNodoErrorSintactico(yytext, linea);
        }
        else{
            // Encuentra el ultimo nodo en la lista y concatena yytext a su errorSintactico
            NodoErrorSintactico *actual = *listaSecuenciasLeidas;

            // Recorrer hasta el ultimo nodo
            while (actual->siguiente != NULL){
                actual = actual->siguiente;
            }

            size_t nuevoTamanio = strlen(actual->errorSintactico) + strlen(yytext) + 1;

            // Redimensiona y concatena la nueva cadena
            char *nuevaCadena = realloc(actual->errorSintactico, nuevoTamanio);
            if (nuevaCadena == NULL){
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            actual->errorSintactico = nuevaCadena;
            strcat(actual->errorSintactico, yytext);

            // Actualiza la linea en caso de que el nodo se haya iniciado antes de esta linea
            actual->linea = linea;
        }
    }
}


// ERRORES SEMANTICOS
NodoErroresSemanticos *crearNodoErroresSemanticos(const char *mensaje, const int linea, const int columna){
    NodoErroresSemanticos *nuevo = (NodoErroresSemanticos *)malloc(sizeof(NodoErroresSemanticos));
    nuevo->mensaje = copiarCadena(mensaje);
    nuevo->linea = linea;
    nuevo->columna = columna;
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarErrorSemantico(NodoErroresSemanticos **lista, const char *mensaje, const int linea, const int columna){
    // Crear el nuevo nodo
    NodoErroresSemanticos *nuevoNodo = crearNodoErroresSemanticos(mensaje, linea, columna);

    if (nuevoNodo == NULL){
        fprintf(stderr, "Error al crear el nodo de error semantico.\n");
        return; // Manejo de error
    }

    // Si la lista esta vacia, el nuevo nodo es el primer nodo
    if (*lista == NULL){
        *lista = nuevoNodo;
        return;
    }

    // Si la lista no esta vacia, recorrer hasta el final
    NodoErroresSemanticos *actual = *lista;
    while (actual->siguiente != NULL){
        actual = actual->siguiente;
    }

    // Enlazar el nuevo nodo al final de la lista
    actual->siguiente = nuevoNodo;
}

void imprimirErrorSemantico(NodoErroresSemanticos *lista){
    NodoErroresSemanticos *actual = lista;
    printf("* Listado de errores semanticos:\n");

    if (actual == NULL){
        printf("-\n");
        return;
    }
    while (actual != NULL){
        printf("%d:%d: %s\n", actual->linea, actual->columna, actual->mensaje);
        actual = actual->siguiente;
    }
}

/*void liberarErrorSemantico(NodoErroresSemanticos *lista){
    NodoErroresSemanticos *actual = lista;
    NodoErroresSemanticos *siguiente = NULL;

    while (actual != NULL){
        siguiente = actual->siguiente;
        free(actual->mensaje);
        free(actual);
        actual = siguiente;
    }
}*/

bool esMultiplicable(char *expresion){
    if(strcmp(expresion,"double")==0 || strcmp(expresion,"float")==0 || strcmp(expresion,"int")==0 || strcmp(expresion,"unsigned int")==0 || strcmp(expresion,"long")==0 || strcmp(expresion,"unsigned long")==0 || strcmp(expresion,"short")==0 || strcmp(expresion,"unsigned short")==0){
        return true;
    }
    else{
        return false;
    }
}

char *extraerTipoDato(char *expresion){ //No extrae el tipo de variables que no esten en la TS, por mas que hayan sido declaradas como parametros    
    NodoSimbolo *nodo = buscar_simbolo(expresion);
    if(nodo == NULL){
        return expresion;
    }
    else if(nodo->tipo == VARIABLE){
        NodoVariableDeclarada *elemento = (NodoVariableDeclarada *)nodo->nodo;
        return enumAString1(elemento->tipoDato);
    }
    else if(nodo->tipo == FUNCION){
        NodoFuncion *elemento = (NodoFuncion *)nodo->nodo;
        return enumAString1(elemento->retorno);
    }
}

const char *extraerTipoDato2(char *expresion){ //No extrae el tipo de variables que no esten en la TS, por mas que hayan sido declaradas como parametros    
    NodoSimbolo *nodo = buscar_simbolo(expresion);
    char *retorno;

    if(nodo == NULL){
        return expresion;
    }
    else if(nodo->tipo == VARIABLE){
        NodoVariableDeclarada *elemento = (NodoVariableDeclarada *)nodo->nodo;
        return enumAString1(elemento->tipoDato);
    }
    else if(nodo->tipo == FUNCION){
        NodoFuncion *elemento = (NodoFuncion *)nodo->nodo;
        Parametro *parametros = elemento->listaDeParametros;
        char tipoDato[256];
        snprintf(tipoDato, sizeof(tipoDato), "%s (*)(", enumAString1(elemento->retorno));
        char *parametrosNuevaFuncion = imprimirParametrosSinIdentificador(parametros);
                if (parametrosNuevaFuncion != NULL){
                    // Aniadir los parametros al tipoDato
                    snprintf(tipoDato + strlen(tipoDato), sizeof(tipoDato) - strlen(tipoDato), "%s)", parametrosNuevaFuncion);     
                    free(parametrosNuevaFuncion); // Liberamos la memoria asignada
                }
                else{
                    // Manejo de error en caso de que no se pueda imprimir parametros
                    snprintf(tipoDato + strlen(tipoDato), sizeof(tipoDato) - strlen(tipoDato), "Error al obtener parametros'): %d:%d",
                            nodo->linea, nodo->columna);
                }
        retorno = strdup(tipoDato);
        return retorno;
    }
    return "Tipo desconocido";
}

void validarMultiplicacion(char *expresion1, char *expresion2, int linea, int columna, NodoErroresSemanticos **listaErroresSemanticos){
    char *tipoDato1 = extraerTipoDato(expresion1);
    char *tipoDato2 = extraerTipoDato(expresion2);

    if((!esMultiplicable(tipoDato1) || !esMultiplicable(tipoDato2))){
        char mensaje[256];
        const char *tipoDatoPrint1 =  extraerTipoDato2(expresion1);
        const char *tipoDatoPrint2 = extraerTipoDato2(expresion2);
        snprintf(mensaje, sizeof(mensaje), "Operandos invalidos del operador binario * (tienen '%s' y '%s')", tipoDatoPrint1, tipoDatoPrint2);
        agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
        // Liberar memoria despues del uso
        //if (tipoDatoPrint1 != expresion1) free((void *)tipoDatoPrint1);
        //if (tipoDatoPrint2 != expresion2) free((void *)tipoDatoPrint2);
    }
}



// PARAMETROS
Parametro *crearNodoParametro(EspecificadorTipos especificadorDeclaracion, const char *identificador, int linea, int columna){
    // Crear nuevo nodo y asignar memoria
    Parametro *nuevo = (Parametro *)malloc(sizeof(Parametro));
    if (nuevo == NULL){
        fprintf(stderr, "Error: no se pudo asignar memoria para el nuevo parametro.\n");
        return NULL;
    }
    nuevo->especificadorDeclaracion = especificadorDeclaracion;
    nuevo->identificador = copiarCadena(identificador);
    nuevo->linea = linea;
    nuevo->columna = columna;
    nuevo->siguiente = NULL;

    return nuevo;
}

int contarArgumentos(Parametro *listaDeParametros) {
    int contador = 0;
    while (listaDeParametros != NULL) {
        contador++;
        listaDeParametros = listaDeParametros->siguiente;
    }
    return contador;
}

void agregarParametro(Parametro **listaDeParametros, EspecificadorTipos especificadorDeclaracion, char *identificador, int linea, int columna){
    // Crear el nodo usando la funcion de creacion
    Parametro *nuevo = crearNodoParametro(especificadorDeclaracion, identificador, linea, columna);

    if (nuevo == NULL){
        printf("Error: no se pudo crear el nuevo nodo de parametro.\n");
        return; // En caso de error en la creacion del nodo
    }

    // Agregar el nodo a la lista
    if (*listaDeParametros == NULL){
        *listaDeParametros = nuevo;
    }
    else{
        Parametro *actual = *listaDeParametros;
        while (actual->siguiente != NULL){
            printf("Nodo actual: identificador = %s\n", actual->identificador);
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }   
}

char *imprimirParametros(Parametro *listaDeParametros){
    char *parametros = (char *)malloc(256);
    if (parametros == NULL) {
        printf("Error: No se pudo asignar memoria para los parametros\n");
        return NULL;
    }

    strcpy(parametros, "");
    Parametro *paramActual = listaDeParametros;

    while (paramActual != NULL) {
        // Inicializa lista de elementos para el parametro actual
        ElementosParametro *lista = NULL;
        ElementosParametro *aux = NULL;

        // Agregar cada especificador a la lista de elementos
        if (paramActual->especificadorDeclaracion.esCalificador != VACIO_CALIFICADORTIPO){
            ElementosParametro *nuevoElemento = malloc(sizeof(ElementosParametro));
            nuevoElemento->elemento = (char*)calificadorTipoString[paramActual->especificadorDeclaracion.esCalificador];
            nuevoElemento->siguiente = NULL;
            if (lista == NULL) lista = nuevoElemento;
            else aux->siguiente = nuevoElemento;
            aux = nuevoElemento;
        }
        if (paramActual->especificadorDeclaracion.esAlmacenamiento != VACIO_ESPALMAC){
            ElementosParametro *nuevoElemento = malloc(sizeof(ElementosParametro));
            nuevoElemento->elemento = (char*)especificadorAlmacenamientoString[paramActual->especificadorDeclaracion.esAlmacenamiento];
            nuevoElemento->siguiente = NULL;
            if (lista == NULL) lista = nuevoElemento;
            else aux->siguiente = nuevoElemento;
            aux = nuevoElemento;
        }
        if (paramActual->especificadorDeclaracion.esTipoDato != VACIO_TIPODATO){
            ElementosParametro *nuevoElemento = malloc(sizeof(ElementosParametro));
            nuevoElemento->elemento = (char*)especificadorTiposString[paramActual->especificadorDeclaracion.esTipoDato];
            nuevoElemento->siguiente = NULL;
            if (lista == NULL) lista = nuevoElemento;
            else aux->siguiente = nuevoElemento;
            aux = nuevoElemento;
        }
        if (paramActual->identificador != NULL){
            ElementosParametro *nuevoElemento = malloc(sizeof(ElementosParametro));
            nuevoElemento->elemento = paramActual->identificador;
            nuevoElemento->siguiente = NULL;
            if (lista == NULL) lista = nuevoElemento;
            else aux->siguiente = nuevoElemento;
            aux = nuevoElemento;
        }

        // Concatenar todos los elementos del parametro actual
        aux = lista;
        while (aux != NULL){
            strcat(parametros, aux->elemento);
            if (aux->siguiente != NULL) strcat(parametros, " ");
            ElementosParametro *temp = aux;
            aux = aux->siguiente;
            free(temp);  // Liberar memoria del nodo
        }

        // Agregar ", " entre parametros si no es el ultimo
        if (paramActual->siguiente != NULL) strcat(parametros, ", ");
        
        paramActual = paramActual->siguiente;
    }
    return parametros;
}

int validarListasDeParametros(Parametro* listaDeParametrosEncontrados, Parametro* listaDeParametrosInvocados){
        int i;
        for(i = 0; i < contarArgumentos(listaDeParametrosEncontrados); i++){
            if(listaDeParametrosEncontrados->especificadorDeclaracion.esTipoDato != listaDeParametrosInvocados->especificadorDeclaracion.esTipoDato) {
                return -1;
            }
            listaDeParametrosEncontrados = listaDeParametrosEncontrados->siguiente;
            listaDeParametrosInvocados = listaDeParametrosInvocados->siguiente;
        }
        return i+1;
}

char *imprimirParametrosSinIdentificador(Parametro *listaDeParametros){
    char *parametros = (char *)malloc(256);
    if (parametros == NULL){
        printf("Error: No se pudo asignar memoria para los parametros\n");
        return NULL;
    }

    strcpy(parametros, "");
    Parametro *paramActual = listaDeParametros;

    typedef struct ElementosParametro{
        char *elemento;
        struct ElementosParametro *siguiente;
    } ElementosParametro;

    while (paramActual != NULL){
        // Crear lista temporal de elementos para el parametro actual
        ElementosParametro* lista = malloc(sizeof(ElementosParametro));
        ElementosParametro* aux = lista;

        // Agregar los especificadores de almacenamiento, calificador y tipo de dato, si no estan vacios
        if (paramActual->especificadorDeclaracion.esCalificador != VACIO_CALIFICADORTIPO){
            aux->elemento = (char*)calificadorTipoString[paramActual->especificadorDeclaracion.esCalificador];
            aux->siguiente = malloc(sizeof(ElementosParametro));
            aux = aux->siguiente;
        }
        if (paramActual->especificadorDeclaracion.esAlmacenamiento != VACIO_ESPALMAC){
            aux->elemento = (char*)especificadorAlmacenamientoString[paramActual->especificadorDeclaracion.esAlmacenamiento];
            aux->siguiente = malloc(sizeof(ElementosParametro));
            aux = aux->siguiente;
        }
        if (paramActual->especificadorDeclaracion.esTipoDato != VACIO_TIPODATO){
            aux->elemento =(char*) especificadorTiposString[paramActual->especificadorDeclaracion.esTipoDato];
            aux->siguiente = NULL;  // ultimo elemento
        }

        // Concatenar los elementos del parametro actual
        aux = lista;
        while (aux != NULL) {
            strcat(parametros, aux->elemento);
            if (aux->siguiente != NULL) {
                strcat(parametros, " ");
            }
            aux = aux->siguiente;
        }

        // Liberar la lista temporal
        while (lista != NULL) {
            ElementosParametro *temp = lista;
            lista = lista->siguiente;
            free(temp);
        }

        // Agregar coma si hay otro parametro siguiente
        if (paramActual->siguiente != NULL) {
            strcat(parametros, ", ");
        }

        paramActual = paramActual->siguiente;
    }
    return parametros;
}


// FUNCIONES
NodoFuncion *crearNodoFuncion(Parametro *listaDeParametros, EspecificadorTipos retorno, tipoFuncion tipogramatica){
    // Asignar memoria para un nuevo nodo de tipo NodoFuncion
    NodoFuncion *nuevo = (NodoFuncion *)malloc(sizeof(NodoFuncion));
    if (nuevo == NULL){
        printf("Error: no se pudo asignar memoria para el nuevo nodo de funcion.\n");
        return NULL;
    }

    // Asignar los valores a los campos del NodoFuncion
    nuevo->retorno = retorno;                     // Copiar el especificador de retorno
    nuevo->tipogramatica = tipogramatica;         // Asignar el tipo de funcion (DEFINICION_FUNCION o DECLARACION_FUNCION)
    nuevo->listaDeParametros = listaDeParametros; // Asignar la lista de parametros
    nuevo->siguiente = NULL;                      // Inicializamos el siguiente puntero a NULL

    return nuevo;
}

void agregarFuncion(NodoSimbolo **lista, NodoSimbolo **tablaSimbolos, EspecificadorTipos retorno, NodoSimbolo **nodoGenericoFuncion, const int linea, tipoFuncion tipogramatica, const int columna){   
    if (nodoGenericoFuncion == NULL){
        printf("Error: nodoGenericoFuncion es NULL.\n");
        return;
    }

    (*nodoGenericoFuncion)->linea = linea;
    (*nodoGenericoFuncion)->columna = columna;
    
    NodoSimbolo *nodoEncontrado = buscar_simbolo((*nodoGenericoFuncion)->nombre);
    NodoFuncion *nuevaFuncion = (NodoFuncion *)(*nodoGenericoFuncion)->nodo;
    nuevaFuncion->retorno = retorno;
    nuevaFuncion->tipogramatica = tipogramatica;

    // PUNTO 2
    if(nodoEncontrado != NULL && nodoEncontrado->tipo == VARIABLE){
        NodoVariableDeclarada* variableEncontrada = (NodoVariableDeclarada *)nodoEncontrado->nodo;
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "'%s' redeclarado como un tipo diferente de simbolo\nNota: la declaracion previa de '%s' es de tipo '%s': %d:%d", (*nodoGenericoFuncion)->nombre, (*nodoGenericoFuncion)->nombre, especificadorTiposString[variableEncontrada->tipoDato.esTipoDato], nodoEncontrado->linea, nodoEncontrado->columna);
        agregarErrorSemantico(&listaErroresSemanticos, mensaje, linea, columna);
        return;   
    }
    
    // PUNTO 3
    if(nodoEncontrado != NULL && nodoEncontrado->tipo == FUNCION){
        NodoFuncion* funcionEncontrada = (NodoFuncion *)nodoEncontrado->nodo;
        Parametro *paramDeclaracion = funcionEncontrada->listaDeParametros;
        Parametro *paramDefinicion = nuevaFuncion->listaDeParametros;
        if (paramDeclaracion != NULL && paramDefinicion != NULL && paramDeclaracion->especificadorDeclaracion.esTipoDato != paramDefinicion->especificadorDeclaracion.esTipoDato){

            char mensaje[256];
            snprintf(mensaje, sizeof(mensaje), 
            "conflicto de tipos para '%s'; la ultima es de tipo '%s(%s)'\nNota: la declaracion previa de '%s' es de tipo '%s(%s)': %d:%d", 
            (*nodoGenericoFuncion)->nombre, especificadorTiposString[nuevaFuncion->retorno.esTipoDato], imprimirParametrosSinIdentificador(paramDefinicion), (*nodoGenericoFuncion)->nombre, especificadorTiposString[funcionEncontrada->retorno.esTipoDato], imprimirParametrosSinIdentificador(paramDeclaracion), nodoEncontrado->linea, nodoEncontrado->columna);
            agregarErrorSemantico(&listaErroresSemanticos, mensaje, linea, columna);
            return;
        }
       
        // REDEFINICION DE FUNCION
        NodoSimbolo* definicionAnterior = fueDefinidaAntes(*tablaSimbolos, (*nodoGenericoFuncion)->nombre);
        if (definicionAnterior != NULL && tipogramatica == DEFINICION_FUNCION){
            //SI LA FUNCION YA FUE DEFINIDA ANTES Y SE QUIERE DEFINIR
            char mensaje[256];
            snprintf(mensaje, sizeof(mensaje), "Redefinicion de '%s'\nNota: la definicion previa de '%s' es de tipo '%s(%s)': %d:%d", (*nodoGenericoFuncion)->nombre, (*nodoGenericoFuncion)->nombre, especificadorTiposString[funcionEncontrada->retorno.esTipoDato], imprimirParametrosSinIdentificador(paramDeclaracion), definicionAnterior->linea, definicionAnterior->columna);
            agregarErrorSemantico(&listaErroresSemanticos, mensaje, linea, columna);
            return;
        } else if (definicionAnterior != NULL){
            //SI LA FUNCION YA FUE DEFINIDA ANTES Y SE QUIERE DECLARAR, NO ES UN ERROR SEMANTICO PERO TAMPOCO SE AGREGA A LA TABLA
            return;
        }
    }

    NodoSimbolo * nuevoSimbolo = crearNodoSimbolo((*nodoGenericoFuncion)->nombre, (*nodoGenericoFuncion)->tipo, (*nodoGenericoFuncion)->linea, (*nodoGenericoFuncion)->columna, (NodoFuncion*)(*nodoGenericoFuncion)->nodo);

    // Si la lista esta vacia, el nuevo NodoSimbolo es el primer NodoSimbolo
    if (*tablaSimbolos == NULL){
        *tablaSimbolos = nuevoSimbolo;
    } else{ // Si la lista no esta vacia, recorrer hasta el final
        NodoSimbolo *actual_simbolo = *tablaSimbolos;
        while (actual_simbolo->siguiente != NULL){
            actual_simbolo = actual_simbolo->siguiente;
        }
        // Enlazar el nuevo nodo al final de la lista
        actual_simbolo->siguiente = nuevoSimbolo;
    }

    free(*nodoGenericoFuncion);
    (*nodoGenericoFuncion) = NULL;
}

NodoSimbolo* fueDefinidaAntes(NodoSimbolo *tablaSimbolos, char* nombre) {
    NodoSimbolo *nodoActual = tablaSimbolos;

    while (nodoActual != NULL) {
        // Verificar si es una funcion y si coincide el nombre
        if (nodoActual->tipo == FUNCION) {
            NodoFuncion *nodoFuncion = (NodoFuncion *)(nodoActual->nodo);

            // Comparar el nombre de la funcion y verificar si ya fue definida
            if (strcmp(nodoActual->nombre, nombre) == 0 && nodoFuncion->tipogramatica == DEFINICION_FUNCION) {
                return nodoActual; // Retornar el nodo si ya esta definida
            }
        }
        nodoActual = nodoActual->siguiente; // Avanzar al siguiente nodo
    }
    return NULL; // Retornar NULL si no se encuentra ninguna definicion previa
}

void imprimirFunciones(NodoSimbolo *lista){
    NodoSimbolo *actual = lista;

    printf("* Listado de funciones declaradas y definidas:\n");

    if (actual == NULL){
        printf("-\n");
        return;
    }

    while (actual != NULL && actual->tipo == FUNCION){
        NodoFuncion *funcion = (NodoFuncion *)actual->nodo;

        const char* esCalificador = (funcion->retorno.esCalificador == VACIO_CALIFICADORTIPO) ? "" : calificadorTipoString[funcion->retorno.esCalificador];
        const char* esAlmacenamiento = (funcion->retorno.esAlmacenamiento == VACIO_ESPALMAC) ? "" : especificadorAlmacenamientoString[funcion->retorno.esAlmacenamiento];
        const char* esTipoDato = (funcion->retorno.esTipoDato == VACIO_TIPODATO) ? "" : especificadorTiposString[funcion->retorno.esTipoDato];

        printf("%s: %s, input: ", actual->nombre, tipoFuncionString[funcion->tipogramatica]);

        // Invocar la funcion para obtener los parametros
        char *parametros = imprimirParametros(funcion->listaDeParametros);
        if (parametros != NULL){
            printf("%s", parametros);
            free(parametros); // Liberar memoria despues de usar
        }
        else{
            printf("Error al obtener los parametros");
        }

        printf(", retorna: %s%s%s", esCalificador, esAlmacenamiento, esTipoDato);
        
        
        printf(", linea %d\n", actual->linea);
        free(funcion);

        actual = actual->siguiente;

        while (actual != NULL && actual->tipo != FUNCION){
            actual = actual->siguiente;
        }
    }
}

/*void liberarFunciones(NodoFuncion *lista){
    NodoFuncion *actual = lista;
    NodoFuncion *siguiente = NULL;

    while (actual != NULL){
        siguiente = actual->siguiente;
        free(actual->funcion);
        free(actual);
        actual = siguiente;
    }
}*/

void validarInvocacionAFuncion(NodoErroresSemanticos **listaErroresSemanticos, char *identificador, Parametro *listaDeParametros, int linea, int columna, int cantidadDeParametros){
    NodoSimbolo *funcion = buscar_simbolo(identificador);
    
    if(funcion == NULL){
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "Funcion '%s' sin declarar", identificador);
        agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
        return;
    }

    if(funcion->tipo != FUNCION){
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "El objeto invocado '%s' no es una funcion o un puntero a una funcion\nNota: declarado aqui: %d:%d", identificador, funcion->linea, funcion->columna);
        agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
        return;
    }

    int cantidadDeParametrosInvocada = cantidadDeParametros;
    int cantidadDeParametrosEncontrada = contarArgumentos(((NodoFuncion *)(funcion)->nodo)->listaDeParametros);

    if (cantidadDeParametrosInvocada > cantidadDeParametrosEncontrada){
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "Demasiados argumentos para la funcion '%s'\nNota: declarado aqui: %d:%d", identificador, funcion->linea, funcion->columna);
        agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
        return;
    }

    if (cantidadDeParametrosInvocada < cantidadDeParametrosEncontrada){
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "Insuficientes argumentos para la funcion '%s'\nNota: declarado aqui: %d:%d", identificador, funcion->linea, funcion->columna);
        agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
        return;
    }
    /* PUNTO 5: ESTO ESTA COMENTADO PORQUE LISTA DE PARAMETROS DE LA INVOCACION NO ESTA FUNCIONANDO
    int numeroDeParametro = validarListasDeParametros(((NodoFuncion *)(funcion)->nodo)->listaDeParametros, listaDeParametros);
    if(numeroDeParametro != -1){
        Parametro *parametroEncontrado = ((NodoFuncion *)(funcion)->nodo)->listaDeParametros;
        for(int i = 0; i < numeroDeParametro; i++){
            parametroEncontrado = parametroEncontrado->siguiente;
        }
        Parametro *parametroInvocado = listaDeParametros;
        for(int i = 0; i < numeroDeParametro; i++){
            parametroInvocado = parametroInvocado->siguiente;
        }
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "Incompatibilidad de tipos para el argumento %d de '%s'\nNota: se esperaba '%s' pero el argumento es de tipo '%s': %d:%d", numeroDeParametro, identificador, tipoFuncionString[parametroEncontrado->especificadorDeclaracion.esTipoDato], tipoFuncionString[parametroInvocado->especificadorDeclaracion.esTipoDato], parametroEncontrado->linea, parametroEncontrado->columna);
        agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
        return;
    }
    */
}

void llenarNodoGenericoFuncion(NodoSimbolo **nodoGenericoFuncion, const char *identificador, Parametro **listaDeParametros){
    // Asignacion de EspecificadorTipos vacio
    EspecificadorTipos especificadorVacio = {.esTipoDato = VACIO_TIPODATO, .esAlmacenamiento = VACIO_ESPALMAC, .esCalificador = VACIO_CALIFICADORTIPO};

    // Crear el nodo de tipo NodoFuncion utilizando la funcion crearNodoFuncion
    NodoFuncion *nuevaFuncion = crearNodoFuncion(*listaDeParametros, especificadorVacio, OTRO);

    if (nuevaFuncion == NULL){
        printf("Error al crear la funcion\n");
        return;
    }

    // Imprimir los datos del nodo de funcion
    Parametro *paramActual = *listaDeParametros;
    while (paramActual != NULL){
        paramActual = paramActual->siguiente;
    }

    // Crear el nodo de tipo NodoSimbolo para almacenar la funcion
    *nodoGenericoFuncion = crearNodoSimbolo(identificador, FUNCION, -1, -1, nuevaFuncion);

    // Comprobacion de la creacion del nodo NodoSimbolo
    if (*nodoGenericoFuncion == NULL){
        printf("Error al crear el NodoSimbolo para la funcion\n");
        return;
    }

    // Limpiar la lista de parametros
    *listaDeParametros = NULL;
}

char *asignarTipoDatoFuncion(char* identificador){
    NodoSimbolo* encontrada = buscar_simbolo(identificador); 
    if(encontrada != NULL){
        return especificadorTiposString[((NodoFuncion *)(encontrada->nodo))->retorno.esTipoDato];
    }
    return "vacio";
}

void ValidarInicializacionVoid(char* tipoDato, int linea, int columna){
    if(strcmp(tipoDato, "void") == 0){
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), "No se ignora el valor de retorno void como deberia ser");
        agregarErrorSemantico(&listaErroresSemanticos, mensaje, linea, columna);
        return;
    }
}


// VARIABLES DECLARADAS
NodoVariableDeclarada *crearNodoVariableDeclarada(EspecificadorTipos tipoDato){
    NodoVariableDeclarada *nuevo = (NodoVariableDeclarada *)malloc(sizeof(NodoVariableDeclarada));
    nuevo->tipoDato = tipoDato;
    return nuevo;
}

void agregarVariableDeclarada(NodoSimbolo **tablaSimbolos, NodoErroresSemanticos **listaErroresSemanticos, char *identificador, EspecificadorTipos tipoDato, int linea, int columna){
    NodoSimbolo *nodoPrevio = buscar_simbolo(identificador);
    if (nodoPrevio == NULL){
        // Crear el nuevo nodo
        NodoVariableDeclarada *nuevoNodo = crearNodoVariableDeclarada(tipoDato);

        // Crear el nuevo NodoSimbolo
        NodoSimbolo *nuevoNodoSimbolo = crearNodoSimbolo(identificador, VARIABLE, linea, columna, nuevoNodo);

        // Si la lista esta vacia, el nuevo NodoSimbolo es el primer NodoSimbolo
        if (*tablaSimbolos == NULL){
            *tablaSimbolos = nuevoNodoSimbolo;
            return;
        }

        // Si la lista no esta vacia, recorrer hasta el final
        NodoSimbolo *actual_simbolo = *tablaSimbolos;
        while (actual_simbolo->siguiente != NULL){
            actual_simbolo = actual_simbolo->siguiente;
        }

        // Enlazar el nuevo nodo al final de la lista
        actual_simbolo->siguiente = nuevoNodoSimbolo;
    }
    else{
        if (nodoPrevio->tipo == VARIABLE){
            NodoVariableDeclarada *elemento = (NodoVariableDeclarada *)nodoPrevio->nodo;
            EspecificadorTipos tipoDatoPrevio = elemento->tipoDato;

            if (tipoDato.esTipoDato != tipoDatoPrevio.esTipoDato){
                char mensaje[256];
                snprintf(mensaje, sizeof(mensaje), "conflicto de tipos para '%s'; la ultima es de tipo '%s'\nNota: la declaracion previa de '%s' es de tipo '%s': %d:%d", identificador, enumAString2(tipoDato), identificador, enumAString2(tipoDatoPrevio), nodoPrevio->linea, nodoPrevio->columna);
                agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
            }
            else{
                char mensaje[256];
                snprintf(mensaje, sizeof(mensaje), "Redeclaracion de '%s'\nNota: la declaracion previa de '%s' es de tipo '%s': %d:%d", identificador, identificador, enumAString2(tipoDatoPrevio), nodoPrevio->linea, nodoPrevio->columna);
                agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
            }
        }
        else if (nodoPrevio->tipo == FUNCION){
            NodoFuncion *elemento = (NodoFuncion *)nodoPrevio->nodo;
            char mensaje[512]; // Aumentamos el tamano del buffer si es necesario
            snprintf(mensaje, sizeof(mensaje), "'%s' redeclarado como un tipo diferente de simbolo\nNota: la declaracion previa de '%s' es de tipo '%s(",
                     identificador, identificador, enumAString2(elemento->retorno));

            char *parametros = imprimirParametros(elemento->listaDeParametros);
            if (parametros != NULL){
                // Aniadir los parametros al mensaje
                snprintf(mensaje + strlen(mensaje), sizeof(mensaje) - strlen(mensaje), "%s)': %d:%d",
                         parametros, nodoPrevio->linea, nodoPrevio->columna);
                free(parametros); // Liberamos la memoria asignada
            }
            else{
                // Manejo de error en caso de que no se pueda imprimir parametros
                snprintf(mensaje + strlen(mensaje), sizeof(mensaje) - strlen(mensaje), "Error al obtener parametros'): %d:%d",
                         nodoPrevio->linea, nodoPrevio->columna);
            }

            agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
        }
    }
}

void imprimirVariables(NodoSimbolo *lista){
    NodoSimbolo *actual = lista;

    printf("* Listado de variables declaradas (tipo de dato y numero de linea):\n");

    if (actual == NULL){
        printf("-\n");
        return;
    }

    while (actual != NULL && actual->tipo != VARIABLE){
        actual = actual->siguiente;
    }

    while (actual != NULL && actual->tipo == VARIABLE){
        NodoVariableDeclarada *variable = (NodoVariableDeclarada *)actual->nodo;
        printf("%s: %s, linea %d, columna %d\n", actual->nombre, enumAString2(variable->tipoDato), actual->linea, actual->columna);
        actual = actual->siguiente;
        while (actual != NULL && actual->tipo != VARIABLE){
            actual = actual->siguiente;
        }
    }
}

void agregarIdentificadorTemporal(IdentificadorTemporal **listaTemporalIdentificadores, char *identificador, int linea, int columna){
    IdentificadorTemporal *nuevo = (IdentificadorTemporal *)malloc(sizeof(IdentificadorTemporal));
    if (nuevo == NULL){
        printf("Error: no se pudo asignar memoria para el nuevo IdentificadorTemporal.\n");
    }

    // Asignar valores a los campos del nodo
    nuevo->identificador = copiarCadena(identificador);
    nuevo->linea = linea;
    nuevo->columna = columna;
    nuevo->siguiente = NULL;

    // Agregar el nodo a la lista
    if (*listaTemporalIdentificadores == NULL){
        *listaTemporalIdentificadores = nuevo;
    }
    else{
        IdentificadorTemporal *actual = *listaTemporalIdentificadores;
        while (actual->siguiente != NULL){
            actual = actual->siguiente;
        }
        actual->siguiente = nuevo;
    }
}

void agregarListaVariables(IdentificadorTemporal *listaTemporalIdentificadores, EspecificadorTipos tipoDato){
    IdentificadorTemporal* actual = listaTemporalIdentificadores;  // Apuntar al primer nodo de la lista

    // Recorrer la lista mientras el nodo actual no sea NULL
    while (actual != NULL){
        agregarVariableDeclarada(&tablaSimbolos, &listaErroresSemanticos, actual->identificador, tipoDato, actual->linea, actual->columna);  // Pasar el nodo actual a la funcion
        actual = actual->siguiente;  // Moverse al siguiente nodo
    }
}

void validarUsoDeVariable(NodoErroresSemanticos **listaErroresSemanticos, char *identificador, char *contextoActual, int linea, int columna, IdentificadorTemporal* listaTemporalIdentificadores){
    bool estaSiendoDeclarada = false;
    IdentificadorTemporal* actual = listaTemporalIdentificadores;
    while (actual != NULL){
        if (strcmp(actual->identificador, identificador) == 0){
            estaSiendoDeclarada = true;
        }
        actual = actual->siguiente;  // Moverse al siguiente nodo
    }
    if (!estaSiendoDeclarada){
        NodoSimbolo *nodoPrevio = buscar_simbolo(identificador);
        if (nodoPrevio == NULL && strcmp(contextoActual,"main")==0){
            char mensaje[256];
            snprintf(mensaje, sizeof(mensaje), "'%s' sin declarar", identificador);
            agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
        }
    }
}

const char *enumAString1(EspecificadorTipos tipoDato){
    // Asigna memoria para el buffer dinamico
    char *buffer = (char *)malloc(150 * sizeof(char));
    if (buffer == NULL){
        fprintf(stderr, "Error: No se pudo asignar memoria para el buffer.\n");
        return NULL;
    }

    const char *tipoStr;

    // Convertir esTipoDato
    switch (tipoDato.esTipoDato){
        case VACIO_TIPODATO: tipoStr = ""; break;
        case CHAR_TIPODATO: tipoStr = "char"; break;
        case VOID_TIPODATO: tipoStr = "void"; break;
        case DOUBLE_TIPODATO: tipoStr = "double"; break;
        case FLOAT_TIPODATO: tipoStr = "float"; break;
        case INT_TIPODATO: tipoStr = "int"; break;
        case UNSIGNED_INT_TIPODATO: tipoStr = "unsigned int"; break;
        case LONG_TIPODATO: tipoStr = "long"; break;
        case UNSIGNED_LONG_TIPODATO: tipoStr = "unsigned long"; break;
        case SHORT_TIPODATO: tipoStr = "short"; break;
        case UNSIGNED_SHORT_TIPODATO: tipoStr = "unsigned short"; break;
    }

    return tipoStr;
}

char *enumAString2(EspecificadorTipos tipoDato){
    // Asigna memoria para el buffer dinamico
    char *buffer = (char *)malloc(150 * sizeof(char));
    if (buffer == NULL){
        fprintf(stderr, "Error: No se pudo asignar memoria para el buffer.\n");
        return NULL;
    }

    const char *tipoStr, *calificadorStr;

    // Convertir esTipoDato
    switch (tipoDato.esTipoDato){
        case VACIO_TIPODATO: tipoStr = ""; break;
        case CHAR_TIPODATO: tipoStr = "char"; break;
        case VOID_TIPODATO: tipoStr = "void"; break;
        case DOUBLE_TIPODATO: tipoStr = "double"; break;
        case FLOAT_TIPODATO: tipoStr = "float"; break;
        case INT_TIPODATO: tipoStr = "int"; break;
        case UNSIGNED_INT_TIPODATO: tipoStr = "unsigned int"; break;
        case LONG_TIPODATO: tipoStr = "long"; break;
        case UNSIGNED_LONG_TIPODATO: tipoStr = "unsigned long"; break;
        case SHORT_TIPODATO: tipoStr = "short"; break;
        case UNSIGNED_SHORT_TIPODATO: tipoStr = "unsigned short"; break;
    }

    // Convertir esCalificador
    switch (tipoDato.esCalificador){
        case CONST_CALIFICADORTIPO: calificadorStr = "const"; break;
        case VOLATILE_CALIFICADORTIPO: calificadorStr = "volatile"; break;
        case VACIO_CALIFICADORTIPO: calificadorStr = ""; break;
    }

    if (tipoDato.esCalificador != 2)    {
        snprintf(buffer, 150, "%s %s", calificadorStr, tipoStr);
    }
    else{
        snprintf(buffer, 150, tipoStr);
    }

    return buffer;
}


// FUNCIONES AUXILIARES DE UTILIDAD
char *copiarCadena(const char *str){
    if (str == NULL){ // Verifica si la cadena es NULL
        return NULL; // Devuelve NULL si la cadena de entrada es NULL
    }

    size_t len = strlen(str);                // Obtiene la longitud de la cadena de entrada
    char *copiado = (char *)malloc(len + 1); // Asigna memoria para la nueva cadena
    if (copiado != NULL){
        strcpy(copiado, str); // Copia el contenido de la cadena de entrada a la nueva cadena
    }
    return copiado; // Devuelve la nueva cadena
}



/*-------------------------------------------------------------------------------------------------------------------*/
// //validacion de asignacion 
// // Funcion para verificar que sea L-Modificable 
// bool esValorModificable(NodoSimbolo *simbolo) {
//     // Verificar si el simbolo es una variable
//     // if (simbolo->tipo != VARIABLE) {
//     //     return false;  // No es una variable, por lo tanto, no es modificable
//     // }

//     // Obtener la informacion de tipo del simbolo
//     NodoVariableDeclarada *variable = (NodoVariableDeclarada *)simbolo->nodo;

//     // Verificar si tiene el calificador const
//     if (variable->tipoDato.esCalificador == CONST_CALIFICADORTIPO) {
//         return false;  // Es const, por lo tanto, no es modificable
//     }

//     // Cumple con las condiciones para ser un valor-L modificable
//     return true;
// }

// void validarAsignacion(NodoErroresSemanticos **listaErroresSemanticos, NodoSimbolo *simboloLadoIzq,  EspecificadorTipos tipoLadoDer, bool esValorModificable, int linea, int columna) {

//     // 1. Validar incompatibilidad de tipos en la inicializacion
// //este no estoy seguro si funciona el resto , creo que si 
//     NodoVariableDeclarada *variable = (NodoVariableDeclarada *)simboloLadoIzq->nodo;
//     // if (variable->tipoDato.esTipoDato != tipoLadoDer.esTipoDato) {
//     //     char mensaje[256];
//     //     snprintf(mensaje, sizeof(mensaje), "%d:%d: Incompatibilidad de tipos al inicializar el tipo '%s' usando el tipo '%s'",  linea, columna, enumAString2(variable->tipoDato), enumAString2(tipoLadoDer));
//     //     agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
//     // }

//     // 2. Validar reasignacion de variable const
//     if (variable->tipoDato.esCalificador == CONST_CALIFICADORTIPO) {
//         char mensaje[256];
//         snprintf(mensaje, sizeof(mensaje),"%d:%d: Asignacion de la variable de solo lectura '%s'", linea, columna, simboloLadoIzq->nombre);
//         agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
//     }

//     // 3. Validar que el lado izquierdo es un valor-L modificable
//     if (!esValorModificable) {
//         char mensaje[256];
//         snprintf(mensaje, sizeof(mensaje), "%d:%d: Se requiere un valor-L modificable como operando izquierdo de la asignacion", linea, columna);
//         agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
//     }
// }

// //validacion sentencia return 
// void validarSentenciaReturn(NodoErroresSemanticos **listaErroresSemanticos,  EspecificadorTipos tipoRetornoDeclarado, EspecificadorTipos tipoReturnSentencia,  int lineaReturn, int columnaReturn, int lineaDeclaracion, int columnaDeclaracion) {  // Caso 1: Se espera un tipo de retorno distinto de void pero se usa un return sin valor
//     if (tipoRetornoDeclarado.esTipoDato != VOID_TIPODATO && tipoReturnSentencia.esTipoDato == VACIO_TIPODATO) {
//      char mensaje[256];
//         snprintf(mensaje, sizeof(mensaje), "%d:%d: 'return' sin valor en una funcion que no retorna void\nNota: declarado aqui: %d:%d", lineaReturn, columnaReturn, lineaDeclaracion, columnaDeclaracion);
//         agregarErrorSemantico(listaErroresSemanticos, mensaje, lineaReturn, columnaReturn);
//         return;
//     }
//     // Caso 2: Incompatibilidad de tipos entre el valor retornado y el tipo de retorno declarado
//     //la condicion creo q hay chequear pero el resto es lo mismo que los errores , si terminan de reavisar la condicion creo q esta
//     if (tipoRetornoDeclarado.esTipoDato != tipoReturnSentencia.esTipoDato) {
//         char mensaje[256];
//         snprintf(mensaje, sizeof(mensaje),"%d:%d: Incompatibilidad de tipos al retornar el tipo '%s' pero se esperaba '%s'",  lineaReturn, columnaReturn, enumAString2(tipoReturnSentencia),enumAString2(tipoRetornoDeclarado));  
//         agregarErrorSemantico(listaErroresSemanticos, mensaje, lineaReturn, columnaReturn);
//     }
// }