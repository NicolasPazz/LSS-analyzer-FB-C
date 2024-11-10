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
/*
// VARIABLES DECLARADAS
NodoSimbolo *crearNodoVariableDeclarada(const char *variableDeclarada, EspecificadorTipos tipoDato, const int linea, const int columna){
    NodoSimbolo *nuevoSimbolo = (NodoSimbolo *)malloc(sizeof(NodoSimbolo));
    NodoVariableDeclarada *nuevaVariableDeclarada = (NodoVariableDeclarada *)malloc(sizeof(NodoVariableDeclarada));
    nuevoSimbolo->nombre = copiarCadena(variableDeclarada);
    nuevoSimbolo->tipo = VARIABLE;
    nuevoSimbolo->linea = linea;
    nuevoSimbolo->columna = columna;
    nuevoSimbolo->nodo = nuevaVariableDeclarada;
    nuevaVariableDeclarada->tipoDato = tipoDato;
    nuevoSimbolo->siguiente = NULL;
    return nuevoSimbolo;
}

void agregarVariableDeclarada(NodoVariableDeclarada **lista, NodoSimbolo **tablaSimbolos, NodoErroresSemanticos **listaErroresSemanticos, char *variableDeclarada, EspecificadorTipos tipoDato, const int linea, const int columna, const char *sufijo){
    NodoSimbolo *nodoPrevio = buscar_simbolo(variableDeclarada);
    if (nodoPrevio == NULL){
        // Crear el nuevo nodo
        NodoVariableDeclarada *nuevoNodo = crearNodoVariableDeclarada(variableDeclarada, tipoDato, linea, columna);

        // Si la lista esta vacia, el nuevo nodo es el primer nodo
        if (*lista == NULL){
            *lista = nuevoNodo;
            return;
        }

        // Si la lista no esta vacia, recorrer hasta el final
        NodoVariableDeclarada *actual = *lista;
        while (actual->siguiente != NULL){
            actual = actual->siguiente;
        }

        // Enlazar el nuevo nodo al final de la lista
        actual->siguiente = nuevoNodo;

        // Crear el nuevo NodoSimbolo
        NodoSimbolo *nuevoNodoSimbolo = crearNodoSimbolo(variableDeclarada, VARIABLE, nuevoNodo);

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
            char mensaje[256];
            snprintf(mensaje, sizeof(mensaje), "'%s' redeclarado como un tipo diferente de simbolo \n Nota: la declaracion previa de '%s' es de tipo '%s': %d:%d", variableDeclarada, variableDeclarada, elemento->tipoDato, elemento->linea, elemento->columna);
            agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
        }
        else if (nodoPrevio->tipo == FUNCION){
            NodoFuncion *elemento = nodoPrevio->nodo;
            char mensaje[512]; // Aumentamos el tamano del buffer si es necesario
            snprintf(mensaje, sizeof(mensaje), "'%s' redeclarado como un tipo diferente de simbolo\nNota: la declaracion previa de '%s' es de tipo '%s(",
                     variableDeclarada, variableDeclarada, elemento->retorno);

            char *parametros = imprimirParametros(elemento->listaDeParametros);
            if (parametros != NULL){
                // Aniadir los parametros al mensaje
                snprintf(mensaje + strlen(mensaje), sizeof(mensaje) - strlen(mensaje), "%s)': %d:%d",
                         parametros, elemento->linea, elemento->columna);
                free(parametros); // Liberamos la memoria asignada
            }
            else{
                // Manejo de error en caso de que no se pueda imprimir parametros
                snprintf(mensaje + strlen(mensaje), sizeof(mensaje) - strlen(mensaje), "Error al obtener parametros'): %d:%d",
                         elemento->linea, elemento->columna);
            }

            agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
        }
    }
}
*/
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
    [UNSIGNED_SHORT_TIPODATO] = "unsigned short"};
const char *especificadorAlmacenamientoString[] = {
    [AUTO_ESPALMAC] = "auto",
    [REGISTER_ESPALMAC] = "register",
    [STATIC_ESPALMAC] = "static",
    [EXTERN_ESPALMAC] = "extern",
    [TYPEDEF_ESPALMAC] = "typedef",
    [VACIO_ESPALMAC] = "vacio"};
const char *calificadorTipoString[] = {
    [CONST_CALIFICADORTIPO] = "const",
    [VOLATILE_CALIFICADORTIPO] = "volatile",
    [VACIO_CALIFICADORTIPO] = "vacio"};

char *imprimirParametros(Parametro *listaDeParametros)
{
    char *parametros = (char *)malloc(256);
    if (parametros == NULL)
    {
        printf("Error: No se pudo asignar memoria para los parametros\n");
        return NULL;
    }

    strcpy(parametros, "");
    Parametro *paramActual = listaDeParametros;

    while (paramActual != NULL)
    {
        // Agregar tipo de dato, almacenamiento y calificador si no están vacíos
        if (paramActual->especificadorDeclaracion.esTipoDato != VACIO_TIPODATO)
        {
            strcat(parametros, especificadorTiposString[paramActual->especificadorDeclaracion.esTipoDato]);
            strcat(parametros, " ");
        }
        if (paramActual->especificadorDeclaracion.esAlmacenamiento != VACIO_ESPALMAC)
        {
            strcat(parametros, especificadorAlmacenamientoString[paramActual->especificadorDeclaracion.esAlmacenamiento]);
            strcat(parametros, " ");
        }
        if (paramActual->especificadorDeclaracion.esCalificador != VACIO_CALIFICADORTIPO)
        {
            strcat(parametros, calificadorTipoString[paramActual->especificadorDeclaracion.esCalificador]);
            strcat(parametros, " ");
        }

        // Agregar identificador
        if (paramActual->identificador != NULL)
        {
            strcat(parametros, paramActual->identificador);
        }

        // Agregar coma y espacio si hay más parámetros
        if (paramActual->siguiente != NULL)
        {
            strcat(parametros, ", ");
        }

        paramActual = paramActual->siguiente;
    }

    return parametros;
}

/*
void imprimirVariablesDeclaradas(NodoVariableDeclarada *lista){
    NodoVariableDeclarada *actual = lista;
    printf("* Listado de variables declaradas (tipo de dato y numero de linea):\n");

    if (actual == NULL){
        printf("-\n");
        return;
    }

    while (actual != NULL){
        if (actual->sufijo != NULL){
            printf("%s: %s %s, linea %d, columna %d\n", actual->variableDeclarada, actual->sufijo, actual->tipoDato, actual->linea, actual->columna);
        }
        else{
            printf("%s: %s, linea %d, columna %d\n", actual->variableDeclarada, actual->tipoDato, actual->linea, actual->columna);
        }
        actual = actual->siguiente;
    }
}

void liberarVariablesDeclaradas(NodoVariableDeclarada *lista){
    NodoVariableDeclarada *actual = lista;
    NodoVariableDeclarada *siguiente = NULL;

    while (actual != NULL){
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}
*/


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
    }
}
/*
void liberarFunciones(NodoFuncion *lista){
    NodoFuncion *actual = lista;
    NodoFuncion *siguiente = NULL;

    while (actual != NULL){
        siguiente = actual->siguiente;
        free(actual->funcion);
        free(actual);
        actual = siguiente;
    }
}
*/
// ESTRUCTURAS NO RECONOCIDAS
NodoErrorSintactico *crearNodoErrorSintactico(const char *errorSintactico, const int linea)
{
    NodoErrorSintactico *nuevo = (NodoErrorSintactico *)malloc(sizeof(NodoErrorSintactico));
    nuevo->errorSintactico = copiarCadena(errorSintactico);
    nuevo->linea = linea;
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarErrorSintactico(NodoErrorSintactico **listaErroresSintacticos, NodoErrorSintactico **listaSecuenciasLeidas)
{
    if (*listaSecuenciasLeidas == NULL)
        return; // No hay nada en la lista de secuencias leidas

    NodoErrorSintactico *nodoError = NULL;
    NodoErrorSintactico *actual = *listaSecuenciasLeidas;

    // Verificar si hay solo un nodo
    if (actual->siguiente == NULL)
    {
        // Si solo hay un nodo, copiar ese nodo
        nodoError = crearNodoErrorSintactico(actual->errorSintactico, actual->linea);
    }
    else
    {
        // Recorrer hasta el anteultimo nodo
        while (actual->siguiente != NULL && actual->siguiente->siguiente != NULL && actual->siguiente->siguiente->siguiente != NULL)
        {
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

void eliminarEspacios(char *cadena)
{
    char *src = cadena; // Puntero para recorrer la cadena
    char *dst = cadena; // Puntero para la posicion de escritura

    // Mover el puntero src hasta el primer caracter no espacio
    while (*src == ' ')
    {
        src++;
    }

    // Copiar el resto de la cadena, omitiendo los espacios al inicio
    while (*src)
    {
        *dst++ = *src++;
    }

    *dst = '\0'; // Terminar la cadena resultante
}

void imprimirErrorSintactico(NodoErrorSintactico *lista)
{
    NodoErrorSintactico *actual = lista;
    printf("* Listado de errores sintacticos:\n");

    if (actual == NULL)
    {
        printf("-\n");
        return;
    }

    while (actual != NULL)
    {
        printf("\"%s\": linea %d\n", actual->errorSintactico, actual->linea);
        actual = actual->siguiente;
    }
}

void liberarErrorSintactico(NodoErrorSintactico *lista)
{
    NodoErrorSintactico *actual = lista;
    NodoErrorSintactico *siguiente = NULL;

    while (actual != NULL)
    {
        siguiente = actual->siguiente;
        free(actual->errorSintactico);
        free(actual);
        actual = siguiente;
    }
}

// CADENAS NO RECONOCIDAS
NodoCadenaNoReconocida *crearNodoCadenaNoReconocida(const char *cadenaNoReconocida, int linea, int columna)
{
    NodoCadenaNoReconocida *nuevo = (NodoCadenaNoReconocida *)malloc(sizeof(NodoCadenaNoReconocida));
    nuevo->cadenaNoReconocida = copiarCadena(cadenaNoReconocida);
    nuevo->linea = linea;
    nuevo->columna = columna;
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarCadenaNoReconocida(NodoCadenaNoReconocida **lista, const char *cadenaNoReconocida, int linea, int columna)
{
    // Crear el nuevo nodo
    NodoCadenaNoReconocida *nuevoNodo = crearNodoCadenaNoReconocida(cadenaNoReconocida, linea, columna);

    // Si la lista esta vacia, el nuevo nodo es el primer nodo
    if (*lista == NULL)
    {
        *lista = nuevoNodo;
        return;
    }

    // Si la lista no esta vacia, recorrer hasta el final
    NodoCadenaNoReconocida *actual = *lista;
    while (actual->siguiente != NULL)
    {
        actual = actual->siguiente;
    }

    // Enlazar el nuevo nodo al final de la lista
    actual->siguiente = nuevoNodo;
}

void imprimirCadenasNoReconocidas(NodoCadenaNoReconocida *lista)
{
    NodoCadenaNoReconocida *actual = lista;
    printf("* Listado de errores lexicos:\n");

    if (actual == NULL)
    {
        printf("-\n");
        return;
    }

    while (actual != NULL)
    {
        printf("%s: linea %d, columna %d\n", actual->cadenaNoReconocida, actual->linea, actual->columna);
        actual = actual->siguiente;
    }
}

void liberarCadenasNoReconocidas(NodoCadenaNoReconocida *lista)
{
    NodoCadenaNoReconocida *actual = lista;
    NodoCadenaNoReconocida *siguiente = NULL;

    while (actual != NULL)
    {
        siguiente = actual->siguiente;
        free(actual->cadenaNoReconocida);
        free(actual);
        actual = siguiente;
    }
}

NodoSimbolo *crearNodoSimbolo(const char *nombre, tipoSimbolo tipo, int linea, int columna, void *nodo){
    // Asignar memoria para el nuevo nodo de tipo NodoSimbolo
    NodoSimbolo *nuevo = (NodoSimbolo *)malloc(sizeof(NodoSimbolo));
    if (nuevo == NULL)
    {
        printf("Error: no se pudo asignar memoria para el nuevo nodo de símbolo.\n");
        return NULL;
    }

    // Asignar los valores a los campos del NodoSimbolo
    nuevo->nombre = copiarCadena(nombre); // Copiar el nombre del símbolo
    nuevo->tipo = tipo;                   // Asignar el tipo del símbolo (VARIABLE o FUNCION)
    nuevo->nodo = nodo;                   // Asignar el nodo asociado (de tipo void*)
    nuevo->linea = linea;                 // Asignar la linea actual leida (donde comienza la funcion)
    nuevo->columna = columna;             // Asignar la columna actual leida (del identificador)
    nuevo->siguiente = NULL;              // Inicializar el siguiente puntero a NULL

    return nuevo; // Retornar el nuevo nodo
}

/*
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
        printf("%d:%d %s\n", actual->linea, actual->columna, actual->mensaje);
        actual = actual->siguiente;
    }
}

void liberarErrorSemantico(NodoErroresSemanticos *lista){
    NodoErroresSemanticos *actual = lista;
    NodoErroresSemanticos *siguiente = NULL;

    while (actual != NULL){
        siguiente = actual->siguiente;
        free(actual->mensaje);
        free(actual);
        actual = siguiente;
    }
}
*/
// Funciones de Utilidad
char *copiarCadena(const char *str)
{
    if (str == NULL)
    {                // Verifica si la cadena es NULL
        return NULL; // Devuelve NULL si la cadena de entrada es NULL
    }

    size_t len = strlen(str);                // Obtiene la longitud de la cadena de entrada
    char *copiado = (char *)malloc(len + 1); // Asigna memoria para la nueva cadena
    if (copiado != NULL)
    {
        strcpy(copiado, str); // Copia el contenido de la cadena de entrada a la nueva cadena
    }
    return copiado; // Devuelve la nueva cadena
}

// Rutinas semanticas
// Validacion de tipos
//  Implementa check_type para manejar los tokens
/*Type check_type(char *token1, char *token2, const int linea, const int columna) {
    // Define las reglas de tipo para multiplicacion (int * int = int, float * float = float, etc.)
    if (strcmp(token1, "IDENTIFICADOR") == 0 || strcmp(token2, "IDENTIFICADOR") == 0) {
        const char *mensaje = "Operandos invalidos del operador binario * (identificador)";
        agregarErrorSemantico(&listaErrorSemantico, mensaje,linea,columna);
        return TIPO_ERROR;
    }
     if (strcmp(token1, "CONSTANTE_ENTERA") == 0 || strcmp(token2, "CONSTANTE_ENTERA") == 0) {
        return TIPO_INT;
    } else  if ((strcmp(token1, "CONSTANTE_ENTERA") == 0 || strcmp(token2, "CONSTANTE_REAL") == 0) &&
               (strcmp(token2, "CONSTANTE_ENTERA") == 0 || strcmp(token1, "CONSTANTE_REAL") == 0)) {
        return TIPO_FLOAT;
    } else if (strcmp(token1, "LITERAL_CADENA") == 0 || strcmp(token2, "LITERAL_CADENA") == 0) {
        const char *mensaje = "Operandos invalidos del operador binario * (literal_cadena)";
        agregarErrorSemantico(&listaErrorSemantico, mensaje, linea,columna);
        return TIPO_ERROR;  // Multiplicacion no soporta strings
    }
    return TIPO_ERROR;  // Devuelve error si no coincide con las reglas
}
*/
void concatenarLeido(NodoErrorSintactico **listaSecuenciasLeidas, const char *yytext, int linea)
{
    // Si es un caracter de corte, crea un nuevo nodo y lo agrega al final de la lista
    if (strcmp(yytext, ";") == 0 || strcmp(yytext, "\n") == 0)
    {
        if (*listaSecuenciasLeidas == NULL)
        {
            // Si la lista esta vacia, crea el primer nodo
            *listaSecuenciasLeidas = crearNodoErrorSintactico("", linea);
        }
        else
        {
            // Si hay nodos, busca el final de la lista
            NodoErrorSintactico *nuevoNodo = crearNodoErrorSintactico("", linea);
            NodoErrorSintactico *actual = *listaSecuenciasLeidas;
            // Recorrer hasta el ultimo nodo
            while (actual->siguiente != NULL)
            {
                actual = actual->siguiente;
            }
            // Enlazar el nuevo nodo al final
            actual->siguiente = nuevoNodo;
        }
    }
    else
    {
        // Si la lista esta vacia, crea el primer nodo
        if (*listaSecuenciasLeidas == NULL)
        {
            *listaSecuenciasLeidas = crearNodoErrorSintactico(yytext, linea);
        }
        else
        {
            // Encuentra el ultimo nodo en la lista y concatena yytext a su errorSintactico
            NodoErrorSintactico *actual = *listaSecuenciasLeidas;

            // Recorrer hasta el ultimo nodo
            while (actual->siguiente != NULL)
            {
                actual = actual->siguiente;
            }

            size_t nuevoTamanio = strlen(actual->errorSintactico) + strlen(yytext) + 1;

            // Redimensiona y concatena la nueva cadena
            char *nuevaCadena = realloc(actual->errorSintactico, nuevoTamanio);
            if (nuevaCadena == NULL)
            {
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
/*
// Insercion de simbolo en la tabla
int insertar_simbolo(char *nombre, tipoSimbolo tipo, void *nodo){
    NodoSimbolo *simbolo_existente = buscar_simbolo(nombre);
    if (simbolo_existente){
        // Ya existe un simbolo con ese nombre

        return -1;
    }
    // Inserta un nuevo simbolo
    NodoSimbolo *nuevo_simbolo = malloc(sizeof(NodoSimbolo));
    nuevo_simbolo->nombre = strdup(nombre);
    nuevo_simbolo->tipo = tipo;
    nuevo_simbolo->nodo = nodo;
    nuevo_simbolo->siguiente = tablaSimbolos;
    tablaSimbolos = nuevo_simbolo;
    return 0;
}*/

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

/*
// Validacion de operaciones especificas
int validar_operacion(NodoSimbolo *simbolo1, NodoSimbolo *simbolo2, char operador){
    if (!simbolo1 || !simbolo2){
        printf("Error: Uno o ambos operandos no estan declarados.\n");
        return -1;
    }

    switch (operador){
    case '*':
        if (simbolo1->tipo == VARIABLE && simbolo2->tipo == VARIABLE){
            // Verifica si ambos son del tipo correcto (int, float, etc.)
            // Por ejemplo, supongamos que nodo contiene el tipo real de la variable
            // Typecasting y verificacion de tipos especificos (a ajustar segun implementacion)
            if (*(int *)simbolo1->nodo != TIPO_INT || *(int *)simbolo2->nodo != TIPO_INT){
                printf("Error: Operandos invalidos para '*'\n");
                return -1;
            }
        }
        break;
        /// Oregarse aqui
    }
    return 0;
}

// Validacion de invocacion de funcion
int validar_invocacion_funcion(NodoSimbolo *simbolo, int num_args)
{ // Esta funcion no la entiendo, hice validarInvocacionAFuncion() que anda bien, pero no borro esta por las dudas
    if (!simbolo || simbolo->tipo != FUNCION){
        printf("Error: '%s' no es una funcion declarada.\n", simbolo->nombre);
        return -1;
    }
    // Validacion especifica para funciones con respecto a los argumentos
    // Tipo de datos de cada argumento a verificar en nodo de la funcion
    return 0;
}

// Validacion de asignacion
int validar_asignacion(NodoSimbolo *simbolo_lado_izq, NodoSimbolo *simbolo_lado_der){
    if (!simbolo_lado_izq || !simbolo_lado_der){
        printf("Error: Variable no declarada en la asignacion.\n");
        return -1;
    }
    if (simbolo_lado_izq->tipo != VARIABLE || simbolo_lado_der->tipo != VARIABLE){
        printf("Error: Asignacion solo permitida entre variables.\n");
        return -1;
    }
    if (simbolo_lado_izq->nodo != simbolo_lado_der->nodo){
        printf("Error: Tipos incompatibles en la asignacion.\n");
        return -1;
    }
    return 0;
}

void validarInvocacionAFuncion(NodoErroresSemanticos **listaErroresSemanticos, char *identificador, char *listaDeArgumentos, int linea, int columna){
    NodoSimbolo *nodoPrevio = buscar_simbolo(identificador);
    if (nodoPrevio == NULL){
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), " Funcion '%s' sin declarar", identificador);
        agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
    }
    else if (nodoPrevio->tipo != FUNCION){
        NodoVariableDeclarada *elemento = (NodoVariableDeclarada *)nodoPrevio->nodo;
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), " El objeto invocado %s no es una funcion o un puntero a una funcion\n Nota: declarado aqui %d:%d", identificador, elemento->linea, elemento->columna);
        agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
    }
    //    else {
    //        char mensaje[256];
    //        snprintf(mensaje, sizeof(mensaje), " Lista de argumentos %d", listaDeArgumentos);
    //        agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
    //    }
}

int contarArgumentos(char *listaDeArgumentos){
    int contador = 0;
    char *token;
    token = strtok(listaDeArgumentos, ",");
    while (token != NULL){
        contador++;
        token = strtok(NULL, ",");
    }
    return contador;
}*/


// FUNCIONES



Parametro *crearNodoParametro(EspecificadorTipos especificadorDeclaracion, const char *identificador){
    // Crear nuevo nodo y asignar memoria
    Parametro *nuevo = (Parametro *)malloc(sizeof(Parametro));
    if (nuevo == NULL)
    {
        fprintf(stderr, "Error: no se pudo asignar memoria para el nuevo parámetro.\n");
        return NULL;
    }
    nuevo->especificadorDeclaracion = especificadorDeclaracion;
    nuevo->identificador = copiarCadena(identificador);
    nuevo->siguiente = NULL;

    return nuevo;
}

void agregarParametro(Parametro **listaDeParametros, EspecificadorTipos especificadorDeclaracion, char *identificador){
    // Crear el nodo usando la funcion de creacion
    Parametro *nuevo = crearNodoParametro(especificadorDeclaracion, identificador);

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

void llenarNodoGenericoFuncion(NodoSimbolo **nodoGenericoFuncion, const char *identificador, Parametro **listaDeParametros){
    // Asignación de EspecificadorTipos vacío
    EspecificadorTipos especificadorVacio = {.esTipoDato = VACIO_TIPODATO, .esAlmacenamiento = VACIO_ESPALMAC, .esCalificador = VACIO_CALIFICADORTIPO};

    // Crear el nodo de tipo NodoFuncion utilizando la función crearNodoFuncion
    NodoFuncion *nuevaFuncion = crearNodoFuncion(*listaDeParametros, especificadorVacio, OTRO);

    if (nuevaFuncion == NULL){
        printf("Error al crear la función\n");
        return;
    }

    // Imprimir los datos del nodo de función
    Parametro *paramActual = *listaDeParametros;
    while (paramActual != NULL){
        paramActual = paramActual->siguiente;
    }

    // Crear el nodo de tipo NodoSimbolo para almacenar la función
    *nodoGenericoFuncion = crearNodoSimbolo(identificador, FUNCION, -1, -1, nuevaFuncion);

    // Comprobación de la creación del nodo NodoSimbolo
    if (*nodoGenericoFuncion == NULL){
        printf("Error al crear el NodoSimbolo para la función\n");
        return;
    }

    if ((*nodoGenericoFuncion) != NULL && (*nodoGenericoFuncion)->nodo != NULL) {
        // Hacer el cast a NodoFuncion
       // NodoFuncion *nodoFuncion = (NodoFuncion *)(*nodoGenericoFuncion)->nodo;
        //char *parametros = imprimirParametros(nodoFuncion->listaDeParametros);
    } else {
        printf("Error: El nodo generico o el nodo de la funcion es NULL\n");
    }

    // Limpiar la lista de parámetros
    *listaDeParametros = NULL;
}

NodoFuncion *crearNodoFuncion(Parametro *listaDeParametros, EspecificadorTipos retorno, tipoFuncion tipogramatica){
    // Asignar memoria para un nuevo nodo de tipo NodoFuncion
    NodoFuncion *nuevo = (NodoFuncion *)malloc(sizeof(NodoFuncion));
    if (nuevo == NULL){
        printf("Error: no se pudo asignar memoria para el nuevo nodo de función.\n");
        return NULL;
    }

    // Asignar los valores a los campos del NodoFuncion
    nuevo->retorno = retorno;                     // Copiar el especificador de retorno
    nuevo->tipogramatica = tipogramatica;         // Asignar el tipo de función (DEFINICION_FUNCION o DECLARACION_FUNCION)
    nuevo->listaDeParametros = listaDeParametros; // Asignar la lista de parámetros
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
  
    //imprimirTablaSimbolos(*tablaSimbolos);
    
    NodoSimbolo *nodoEncontrado = buscar_simbolo((*nodoGenericoFuncion)->nombre);
    NodoFuncion *nuevaFuncion = (NodoFuncion *)(*nodoGenericoFuncion)->nodo;
    nuevaFuncion->retorno = retorno;
    nuevaFuncion->tipogramatica = tipogramatica;
    
    // Si el identificador no fue usado antes o el identificador se uso en una declaracion de funcion y ahora se quiere definir, sin haber sido definida antes
    if (nodoEncontrado == NULL || ((nodoEncontrado != NULL && nodoEncontrado->tipo == FUNCION && ((NodoFuncion *)nodoEncontrado->nodo)->tipogramatica == DECLARACION_FUNCION && nuevaFuncion->tipogramatica == DEFINICION_FUNCION && noFueDefinidaAntes(*tablaSimbolos, *nodoGenericoFuncion)))){
        // Si el identificador se uso en una declaracion de funcion y ahora se quiere definir, sin haber sido definida antes. Validar que los parametros que se pretenden usar en la definicion, sean compatibles con los de la declaracion
        if (nodoEncontrado != NULL && nodoEncontrado->tipo == FUNCION && ((NodoFuncion *)nodoEncontrado->nodo)->tipogramatica == DECLARACION_FUNCION && nuevaFuncion->tipogramatica == DEFINICION_FUNCION && noFueDefinidaAntes(*tablaSimbolos, *nodoGenericoFuncion)){
            Parametro *paramDeclaracion = ((NodoFuncion *)nodoEncontrado->nodo)->listaDeParametros;
            Parametro *paramDefinicion = nuevaFuncion->listaDeParametros;

            while (paramDeclaracion != NULL && paramDefinicion != NULL){
                // Comprobar solo si ambos sufijos no son NULL
                if (paramDeclaracion->especificadorDeclaracion.esTipoDato != VACIO_TIPODATO && paramDefinicion->especificadorDeclaracion.esTipoDato != VACIO_TIPODATO){
                    if (paramDeclaracion->especificadorDeclaracion.esTipoDato != paramDefinicion->especificadorDeclaracion.esTipoDato){
                        // printf("Error: Incompatibilidad de sufijos entre declaracion y definicion en la funcion '%s'.\n", (*nodoGenericoFuncion)->funcion);
                        return;
                    }
                }
                if (paramDeclaracion->especificadorDeclaracion.esAlmacenamiento != VACIO_ESPALMAC && paramDefinicion->especificadorDeclaracion.esAlmacenamiento != VACIO_ESPALMAC){
                    if (paramDeclaracion->especificadorDeclaracion.esAlmacenamiento != paramDefinicion->especificadorDeclaracion.esAlmacenamiento){
                        // printf("Error: Incompatibilidad de sufijos entre declaracion y definicion en la funcion '%s'.\n", (*nodoGenericoFuncion)->funcion);
                        return;
                    }
                }
                if (paramDeclaracion->especificadorDeclaracion.esCalificador != VACIO_CALIFICADORTIPO && paramDefinicion->especificadorDeclaracion.esCalificador != VACIO_CALIFICADORTIPO){
                    if (paramDeclaracion->especificadorDeclaracion.esCalificador != paramDefinicion->especificadorDeclaracion.esCalificador){
                        // printf("Error: Incompatibilidad de sufijos entre declaracion y definicion en la funcion '%s'.\n", (*nodoGenericoFuncion)->funcion);
                        return;
                    }
                }
                paramDeclaracion = paramDeclaracion->siguiente;
                paramDefinicion = paramDefinicion->siguiente;
            }

            if (paramDeclaracion != NULL || paramDefinicion != NULL){
                printf("Error: La cantidad de parametros entre declaracion y definicion no coincide para la funcion '%s'.\n", (*nodoGenericoFuncion)->nombre);
                return;
            }
        }
        NodoSimbolo * nuevoSimbolo = crearNodoSimbolo((*nodoGenericoFuncion)->nombre, (*nodoGenericoFuncion)->tipo, (*nodoGenericoFuncion)->linea, (*nodoGenericoFuncion)->columna, (NodoFuncion*)(*nodoGenericoFuncion)->nodo);

        // Si la lista esta vacia, el nuevo NodoSimbolo es el primer NodoSimbolo
        if (*tablaSimbolos == NULL)
        {
            *tablaSimbolos = nuevoSimbolo;
        }
        else
        { // Si la lista no esta vacia, recorrer hasta el final
            NodoSimbolo *actual_simbolo = *tablaSimbolos;
            while (actual_simbolo->siguiente != NULL)
            {
                actual_simbolo = actual_simbolo->siguiente;
            }
            // Enlazar el nuevo nodo al final de la lista
            actual_simbolo->siguiente = nuevoSimbolo;
        }
    }
    else if (nodoEncontrado->tipo == FUNCION &&
             ((NodoFuncion *)nodoEncontrado->nodo)->tipogramatica == DEFINICION_FUNCION &&
             nuevaFuncion->tipogramatica == DECLARACION_FUNCION)
    {
        // definicion ya existente, y se intenta declarar (sin efecto)
    }
    else if (nodoEncontrado->tipo == FUNCION &&
             ((NodoFuncion *)nodoEncontrado->nodo)->tipogramatica == nuevaFuncion->tipogramatica)
    {
        // redeclaracion o redefinicion de funcion
    }
    else
    {
        // agregarErrorSemantico("%d:%d Funcion '%s' sin declarar", linea, columna, (*nodoGenericoFuncion)->funcion); // SEGMENTATION FAULT
    }
    free(*nodoGenericoFuncion);
    (*nodoGenericoFuncion) = NULL;
}

bool noFueDefinidaAntes(NodoSimbolo *tablaSimbolos, NodoSimbolo *nodoGenericoFuncion)
{
    NodoSimbolo *nodoActual = tablaSimbolos;

    while (nodoActual != NULL)
    {
        if (nodoActual->tipo == FUNCION)
        {
            NodoFuncion *nodoFuncion = (NodoFuncion *)(nodoActual->nodo);

            // Comparar nombre de la funcion y verificar que no haya sido definida antes
            if (nodoActual->nombre == nodoGenericoFuncion->nombre &&
                nodoFuncion->tipogramatica == DEFINICION_FUNCION)
            {
                return false; // Ya fue definida antes
            }
        }
        nodoActual = nodoActual->siguiente; // Avanzar al siguiente simbolo en la tabla
    }

    return true; // No fue definida antes
}


void imprimirTablaSimbolos(NodoSimbolo *tablaSimbolos){
    NodoSimbolo *nodoActual = tablaSimbolos;

    printf("Tabla de Símbolos:\n");
    if (nodoActual == NULL)
    {
      //  printf("La tabla de símbolos está vacía :(\n\n");
    }

    while (nodoActual != NULL)
    {
       // printf("Nombre: %s\n", nodoActual->nombre);
        //printf("Tipo: %s\n", nodoActual->tipo == VARIABLE ? "VARIABLE" : "FUNCION");
        //printf("Linea: %d\n", nodoActual->linea);
        //printf("Columna: %d\n", nodoActual->columna);

        if (nodoActual->tipo == VARIABLE)
        {
            //NodoVariableDeclarada *var = (NodoVariableDeclarada *)nodoActual->nodo;
          /* printf("Tipo de dato: %s, Almacenamiento: %s, Calificador: %s\n",
                   especificadorTiposString[var->tipoDato.esTipoDato],
                   especificadorAlmacenamientoString[var->tipoDato.esAlmacenamiento],
                   calificadorTipoString[var->tipoDato.esCalificador]);*/
        }
        else if (nodoActual->tipo == FUNCION)
        {
            NodoFuncion *func = (NodoFuncion *)nodoActual->nodo;
           /* printf("Retorno: Tipo de dato: %s, Almacenamiento: %s, Calificador: %s\n",
                   especificadorTiposString[func->retorno.esTipoDato],
                   especificadorAlmacenamientoString[func->retorno.esAlmacenamiento],
                   calificadorTipoString[func->retorno.esCalificador]);*/
           // printf("Tipo de gramática: %s\n", func->tipogramatica == DEFINICION_FUNCION ? "DEFINICION" : "DECLARACION");

            // Imprimir parámetros usando tu función
            char *parametros = imprimirParametros(func->listaDeParametros);
           // printf("  Parámetros: %s\n\n", parametros);
            free(parametros); // Liberar memoria después de imprimir
        }

        printf("\n");
        nodoActual = nodoActual->siguiente;
    }
}

/*
// Se recibe el tipo de dato de la funcion y se llama a la funcion copiarCadena
void inicializarTipoRetorno(const char *tipo) {
    tipoReturnEsperado = copiarCadena(tipo);
}

// Se recibe el tipo de dato del return encontrado junto con su fila y columna
// fila y columna funcionan ok
// No se esta recibiendo el tipo de dato del return, si no que se recibe un char* con la cadena del return. Ya sea identificador, literal cadena o constante.
void registrarReturn(const char *tipo, int linea, int columna) {
    if (tipoReturnEncontrado == NULL) {
        tipoReturnEncontrado = (TipoRetorno *)malloc(sizeof(TipoRetorno));
    }
    tipoReturnEncontrado->tipoDato = copiarCadena(tipo);
    tipoReturnEncontrado->linea = linea;
    tipoReturnEncontrado->columna = columna+1;
    tipoReturnEncontrado->siguiente = NULL;
}

// Se valida si el tipo de dato del return y de la funcion son el mismo y se genera accion dependiendo el caso.
// Al ser un strcmp, se esta comparando dos cadenas char*, no segun su valor semantico.
void validarTipoReturn(NodoErroresSemanticos **listaErroresSemanticos) {
    if (tipoReturnEsperado != NULL && tipoReturnEncontrado->tipoDato != NULL) {
        if (strcmp(tipoReturnEsperado, tipoReturnEncontrado->tipoDato) != 0) {
            char mensaje[256];
            snprintf(mensaje, sizeof(mensaje), "Incompatibilidad de tipos al retornar el tipo '%s' pero se esperaba '%s'", tipoReturnEncontrado->tipoDato, tipoReturnEsperado);
            agregarErrorSemantico(listaErroresSemanticos, mensaje, tipoReturnEncontrado->linea, tipoReturnEncontrado->columna);
        }
    }
    free(tipoReturnEncontrado->tipoDato);
    tipoReturnEncontrado->tipoDato = NULL;
}


char* obtenerTipoIdentificador(const char *identificador) {
    NodoSimbolo *simbolo = buscar_simbolo(identificador);
    if (simbolo) {
        if (simbolo->tipo == FUNCION) {
            NodoFuncion *func = (NodoFuncion *)simbolo->nodo;
            return func->retorno;  // Retorna el tipo de retorno de la función, como "void (*)(void)"
        } else if (simbolo->tipo == VARIABLE) {
            NodoVariableDeclarada *var = (NodoVariableDeclarada *)simbolo->nodo;
            return var->tipoDato;  // Retorna el tipo de dato de la variable
        }
    }
    return NULL;
}

NodoVariableDeclarada

int verificarTipoRetorno(tipoDato tipoFuncion, tipoDato tipoReturn) {
    if (tipoReturn == tipoFuncion) {
        return 1;
    } else {
        return 0;
    }
} // puede ir algo por aca la validacion de tipos. En vez de return 1 o 0, se puede generar accion.
*/

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