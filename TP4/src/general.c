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
NodoVariableDeclarada* crearNodoVariableDeclarada(const char *variableDeclarada, const char *tipoDato, const int linea, const int columna, const char *sufijo){
    NodoVariableDeclarada *nuevo = (NodoVariableDeclarada *)malloc(sizeof(NodoVariableDeclarada));
    nuevo->variableDeclarada = copiarCadena(variableDeclarada);
    nuevo->tipoDato = copiarCadena(tipoDato);
    nuevo->linea = linea;
    nuevo->columna = columna;
    nuevo->sufijo = sufijo;
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarVariableDeclarada(NodoVariableDeclarada **lista, NodoSimbolo **tablaSimbolos, NodoErroresSemanticos **listaErroresSemanticos, char *variableDeclarada, const char *tipoDato, const int linea, const int columna, const char *sufijo){
    NodoSimbolo *nodoPrevio = buscar_simbolo(variableDeclarada);
    if (nodoPrevio == NULL){
        // Crear el nuevo nodo
        NodoVariableDeclarada *nuevoNodo = crearNodoVariableDeclarada(variableDeclarada, tipoDato, linea, columna, sufijo);

        // Si la lista está vacía, el nuevo nodo es el primer nodo
        if (*lista == NULL) {
            *lista = nuevoNodo;
            return;
        }

        // Si la lista no esta vacia, recorrer hasta el final
        NodoVariableDeclarada *actual = *lista;
        while (actual->siguiente != NULL) {
            actual = actual->siguiente;
        }

        // Enlazar el nuevo nodo al final de la lista
        actual->siguiente = nuevoNodo;

        // Crear el nuevo NodoSimbolo
        NodoSimbolo *nuevoNodoSimbolo = crearNodoSimbolo(variableDeclarada, VARIABLE, nuevoNodo);

        // Si la lista está vacía, el nuevo NodoSimbolo es el primer NodoSimbolo
        if (*tablaSimbolos == NULL) {
            *tablaSimbolos = nuevoNodoSimbolo;
            return;
        }

        // Si la lista no esta vacia, recorrer hasta el final
        NodoSimbolo *actual_simbolo = *tablaSimbolos;
        while (actual_simbolo->siguiente != NULL) {
            actual_simbolo = actual_simbolo->siguiente;
        }

        // Enlazar el nuevo nodo al final de la lista
        actual_simbolo->siguiente = nuevoNodoSimbolo;
        
    }
    else {
        if (nodoPrevio->tipo == VARIABLE){
            NodoVariableDeclarada *elemento = (NodoVariableDeclarada *)nodoPrevio->nodo;
            char mensaje[256];
            snprintf(mensaje, sizeof(mensaje), "%d:%d '%s' redeclarado como un tipo diferente de simbolo \n Nota: la declaracion previa de '%s' es de tipo '%s': %d:%d", linea, columna, variableDeclarada, variableDeclarada, elemento->tipoDato, elemento->linea, elemento->columna);
            agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
        }
        else if (nodoPrevio->tipo == FUNCION){
            NodoFuncion* elemento = nodoPrevio->nodo;
            char mensaje[256];
            snprintf(mensaje, sizeof(mensaje), "%d:%d '%s' redeclarado como un tipo diferente de simbolo \n Nota: la declaracion previa de '%s' es de tipo '%s': %d:%d", linea, columna, variableDeclarada, variableDeclarada, elemento->retorno, elemento->linea, elemento->columna);
            agregarErrorSemantico(listaErroresSemanticos, mensaje, linea, columna);
        }        
    }
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
            printf("%s: %s %s, linea %d, columna %d\n", actual->variableDeclarada, actual->sufijo, actual->tipoDato, actual->linea, actual->columna);
        }else{
            printf("%s: %s, linea %d, columna %d\n", actual->variableDeclarada, actual->tipoDato, actual->linea, actual->columna);
        }
        actual = actual->siguiente;
    }
}

void liberarVariablesDeclaradas(NodoVariableDeclarada *lista){
    NodoVariableDeclarada *actual = lista;
    NodoVariableDeclarada *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
}

// FUNCIONES
NodoFuncion* crearNodoFuncion(const char *retorno, const char *funcion, const int linea, const char* tipogramatica){
    NodoFuncion *nuevo = (NodoFuncion *)malloc(sizeof(NodoFuncion));
    
    nuevo->funcion = copiarCadena(funcion);
    nuevo->linea = linea;
    nuevo->parametro = copiarCadena(listaParametros);
    listaParametros = NULL;
    nuevo->retorno = copiarCadena(retorno);
    nuevo->tipogramatica = copiarCadena(tipogramatica);
    nuevo->siguiente = NULL;

    return nuevo;
}

char* unirParametros(const char* param1, const char* param2) {
    // Aquí asumimos que ambos parámetros son no nulos

    // Calculamos las longitudes de los parámetros
    size_t longitud1 = strlen(param1);
    size_t longitud2 = strlen(param2);

    // Asignamos memoria para la cadena resultante
    char* resultado = (char*)malloc(longitud1 + longitud2 + 2); // +2 para espacio y terminador nulo
    if (resultado == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // Copiamos el primer parámetro
    strcpy(resultado, param1);

    // Agregamos un espacio
    strcat(resultado, " ");

    // Concatenamos el segundo parámetro
    strcat(resultado, param2);

    return resultado; // Retornamos la cadena resultante
}

void agregarParametro(char** lista, char* parametro) { //enum
    if (*lista == NULL) {
        *lista = copiarCadena(parametro);
    } else {
        size_t newLength = strlen(*lista) + strlen(parametro) + 3;
        char* nuevaLista = (char*)malloc(newLength);
        if (nuevaLista == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }

        strcpy(nuevaLista, *lista);
        strcat(nuevaLista, ", ");
        strcat(nuevaLista, parametro);

        free(*lista);
        *lista = nuevaLista;
    }
}

void agregarFuncion(NodoFuncion **lista, NodoSimbolo **tablaSimbolos, const char *retorno, char *funcion, const int linea, const char* tipogramatica, const int columna){
    if (buscar_simbolo(funcion) == NULL){
    // Crear el nuevo nodo
    NodoFuncion *nuevoNodo = crearNodoFuncion(retorno, funcion, linea, tipogramatica);
    // Si la lista está vacía, el nuevo nodo es el primer nodo
    if (*lista == NULL) {
        *lista = nuevoNodo;
        return;
    }

    // Si la lista no está vacía, recorrer hasta el final
    NodoFuncion *actual = *lista;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    // Enlazar el nuevo nodo al final de la lista
    actual->siguiente = nuevoNodo;

    // Crear el nuevo NodoSimbolo
    NodoSimbolo *nuevoNodoSimbolo = crearNodoSimbolo(funcion, FUNCION, nuevoNodo);

    // Si la lista está vacía, el nuevo NodoSimbolo es el primer NodoSimbolo
    if (*tablaSimbolos == NULL) {
        *tablaSimbolos = nuevoNodoSimbolo;
        return;
    }

    // Si la lista no esta vacia, recorrer hasta el final
    NodoSimbolo *actual_simbolo = *tablaSimbolos;
    while (actual_simbolo->siguiente != NULL) {
        actual_simbolo = actual_simbolo->siguiente;
    }
    
    // Enlazar el nuevo nodo al final de la lista
    actual_simbolo->siguiente = nuevoNodoSimbolo;
   }
    //else{
     //   agregarErrorSemantico("%d:%d Funcion '%s' sin declarar", linea, columna, funcion);
   // }
}

void imprimirFunciones(NodoFuncion *lista) {
    NodoFuncion *actual = lista;
    printf("* Listado de funciones declaradas o definidas:\n");

    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("%s: %s, input: %s, retorna: %s, linea %d\n", actual->funcion, actual->tipogramatica, actual->parametro, actual->retorno, actual->linea);
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

// ESTRUCTURAS NO RECONOCIDAS
NodoErrorSintactico* crearNodoErrorSintactico(const char *errorSintactico, const int linea){
    NodoErrorSintactico *nuevo = (NodoErrorSintactico *)malloc(sizeof(NodoErrorSintactico));
    nuevo->errorSintactico = copiarCadena(errorSintactico);
    nuevo->linea = linea;
    nuevo->siguiente = NULL;
    return nuevo;
}

void agregarErrorSintactico(NodoErrorSintactico **listaErroresSintacticos, NodoErrorSintactico **listaSecuenciasLeidas) {
    if (*listaSecuenciasLeidas == NULL) return; // No hay nada en la lista de secuencias leídas

    NodoErrorSintactico *nodoError = NULL;
    NodoErrorSintactico *actual = *listaSecuenciasLeidas;

    // Verificar si hay solo un nodo
    if (actual->siguiente == NULL) {
        // Si solo hay un nodo, copiar ese nodo
        nodoError = crearNodoErrorSintactico(actual->errorSintactico, actual->linea);
    } else {
        // Recorrer hasta el anteultimo nodo
        while (actual->siguiente->siguiente->siguiente != NULL) {
            actual = actual->siguiente;
        }
        // Ahora `actual` es el anteultimo nodo
        eliminarEspacios(actual->errorSintactico);
        nodoError = crearNodoErrorSintactico(actual->errorSintactico, actual->linea); // Copiar el anteúltimo nodo
    }

    // Agregar el nodo seleccionado a la lista de errores sintácticos
    nodoError->siguiente = *listaErroresSintacticos;
    *listaErroresSintacticos = nodoError;
}

void eliminarEspacios(char *cadena) {
    char *src = cadena; // Puntero para recorrer la cadena
    char *dst = cadena; // Puntero para la posición de escritura

    // Mover el puntero src hasta el primer carácter no espacio
    while (*src == ' ') {
        src++;
    }

    // Copiar el resto de la cadena, omitiendo los espacios al inicio
    while (*src) {
        *dst++ = *src++;
    }

    *dst = '\0'; // Terminar la cadena resultante
}

void imprimirErrorSintactico(NodoErrorSintactico *lista){
    NodoErrorSintactico *actual = lista;
    printf("* Listado de errores sintacticos:\n");
    
    if (actual == NULL) {
        printf("-\n");
        return;
    }

    while (actual != NULL) {
        printf("\"%s\": linea %d\n", actual->errorSintactico, actual->linea);
        actual = actual->siguiente;
    }
}

void liberarErrorSintactico(NodoErrorSintactico *lista){
    NodoErrorSintactico *actual = lista;
    NodoErrorSintactico *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->errorSintactico);
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

    // Si la lista esta vacia, el nuevo nodo es el primer nodo
    if (*lista == NULL) {
        *lista = nuevoNodo;
        return;
    }

    // Si la lista no esta vacia, recorrer hasta el final
    NodoCadenaNoReconocida *actual = *lista;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    // Enlazar el nuevo nodo al final de la lista
    actual->siguiente = nuevoNodo;
}

void  imprimirCadenasNoReconocidas(NodoCadenaNoReconocida *lista) {
   NodoCadenaNoReconocida *actual = lista;
    printf("* Listado de errores lexicos:\n");
    
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

NodoSimbolo* crearNodoSimbolo(const char *nombre, tipoSimbolo tipo, void* nodo){
    NodoSimbolo *nuevo = (NodoSimbolo *)malloc(sizeof(NodoSimbolo));
    nuevo->nombre = copiarCadena(nombre);
    nuevo->tipo = tipo;
    nuevo->nodo = nodo;
    nuevo->siguiente = NULL;
    return nuevo;
}

// ERRORES SEMANTICOS
NodoErroresSemanticos* crearNodoErroresSemanticos(const char *mensaje, const int linea, const int columna){
    NodoErroresSemanticos *nuevo = (NodoErroresSemanticos *)malloc(sizeof(NodoErroresSemanticos));
    nuevo->mensaje = copiarCadena(mensaje);
    nuevo->linea = linea;
    nuevo->columna = columna;
    nuevo->siguiente = NULL;
    return nuevo;
}
NodoSimbolo* buscarSimbolo(const char *nombre) {
    NodoSimbolo *actual = tablaSimbolos; // Empezar desde el inicio de la tabla

    // Recorrer la lista de símbolos
    while (actual != NULL) {
        // Comparar el nombre actual con el nombre buscado
        if (strcmp(actual->nombre, nombre) == 0) {
            return actual; // Símbolo encontrado
        }
        actual = actual->siguiente; // Avanzar al siguiente nodo
    }
    return NULL; // No se encontró el símbolo
}

void agregarErrorSemantico(NodoErroresSemanticos **lista, const char *mensaje, const int linea, const int columna){
    // Crear el nuevo nodo
    NodoErroresSemanticos *nuevoNodo = crearNodoErroresSemanticos(mensaje, linea, columna);
    
    if (nuevoNodo == NULL) {
        fprintf(stderr, "Error al crear el nodo de error semántico.\n");
        return; // Manejo de error
    }

    // Si la lista está vacía, el nuevo nodo es el primer nodo
    if (*lista == NULL) {
        *lista = nuevoNodo;
        return;
    }

    // Si la lista no está vacía, recorrer hasta el final
    NodoErroresSemanticos *actual = *lista;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    // Enlazar el nuevo nodo al final de la lista
    actual->siguiente = nuevoNodo;
}


void imprimirErrorSemantico(NodoErroresSemanticos *lista){
    NodoErroresSemanticos *actual = lista;
    printf("* Listado de errores semanticos:\n");
    
    if (actual == NULL) {
        printf("-\n");
        return;
    }
    while (actual != NULL) {
            printf("%d:%d %s\n", actual->linea, actual->columna, actual->mensaje);
        actual = actual->siguiente;
    }

}

void liberarErrorSemantico(NodoErroresSemanticos *lista){
    NodoErroresSemanticos *actual = lista;
    NodoErroresSemanticos *siguiente = NULL;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual->mensaje);
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

//Rutinas semanticas
//Validacion de tipos 
// Implementa check_type para manejar los tokens
/*Type check_type(char *token1, char *token2, const int linea, const int columna) {
    // Define las reglas de tipo para multiplicación (int * int = int, float * float = float, etc.)
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
        return TIPO_ERROR;  // Multiplicación no soporta strings
    }
    return TIPO_ERROR;  // Devuelve error si no coincide con las reglas
}*/

void concatenarLeido(NodoErrorSintactico **listaSecuenciasLeidas, const char *yytext, int linea) {
    // Si es un carácter de corte, crea un nuevo nodo y lo agrega al final de la lista
    if (strcmp(yytext, ";") == 0 || strcmp(yytext, "\n") == 0) {
        if (*listaSecuenciasLeidas == NULL) {
            // Si la lista está vacía, crea el primer nodo
            *listaSecuenciasLeidas = crearNodoErrorSintactico("", linea);
        } else {
            // Si hay nodos, busca el final de la lista
            NodoErrorSintactico *nuevoNodo = crearNodoErrorSintactico("", linea);
            NodoErrorSintactico *actual = *listaSecuenciasLeidas;
            
            // Recorrer hasta el último nodo
            while (actual->siguiente != NULL) {
                actual = actual->siguiente;
            }
            // Enlazar el nuevo nodo al final
            actual->siguiente = nuevoNodo;
        }
    } else {
        // Si la lista está vacía, crea el primer nodo
        if (*listaSecuenciasLeidas == NULL) {
            *listaSecuenciasLeidas = crearNodoErrorSintactico(yytext, linea);
        } else {
            // Encuentra el último nodo en la lista y concatena yytext a su errorSintactico
            NodoErrorSintactico *actual = *listaSecuenciasLeidas;
            
            // Recorrer hasta el último nodo
            while (actual->siguiente != NULL) {
                actual = actual->siguiente;
            }

            size_t nuevoTamanio = strlen(actual->errorSintactico) + strlen(yytext) + 1;
            
            // Redimensiona y concatena la nueva cadena
            char *nuevaCadena = realloc(actual->errorSintactico, nuevoTamanio);
            if (nuevaCadena == NULL) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            actual->errorSintactico = nuevaCadena;
            strcat(actual->errorSintactico, yytext);

            // Actualiza la línea en caso de que el nodo se haya iniciado antes de esta línea
            actual->linea = linea;
        }
    }
}

// Inserción de símbolo en la tabla
int insertar_simbolo(char *nombre, tipoSimbolo tipo, void *nodo) {
    NodoSimbolo *simbolo_existente = buscar_simbolo(nombre);
    if (simbolo_existente) {
        // Ya existe un símbolo con ese nombre
        return -1;
    }
    // Inserta un nuevo símbolo
    NodoSimbolo *nuevo_simbolo = malloc(sizeof(NodoSimbolo));
    nuevo_simbolo->nombre = strdup(nombre);
    nuevo_simbolo->tipo = tipo;
    nuevo_simbolo->nodo = nodo;
    nuevo_simbolo->siguiente = tablaSimbolos;
    tablaSimbolos = nuevo_simbolo;
    return 0;
}

NodoSimbolo *buscar_simbolo(char *nombre) {
    NodoSimbolo *nodo_actual = tablaSimbolos;
    while (nodo_actual != NULL) {
        if (strcmp(nodo_actual->nombre, nombre) == 0) {
            return nodo_actual; // Retornar el nodo encontrado
        }
        nodo_actual = nodo_actual->siguiente; // Mover al siguiente nodo
    }
    return NULL; // Retornar NULL si no se encuentra el nodo
}

// Validación de operaciones específicas
int validar_operacion(NodoSimbolo *simbolo1, NodoSimbolo *simbolo2, char operador) {
    if (!simbolo1 || !simbolo2) {
        printf("Error: Uno o ambos operandos no están declarados.\n");
        return -1;
    }

    switch (operador) {
        case '*':
            if (simbolo1->tipo == VARIABLE && simbolo2->tipo == VARIABLE) {
                // Verifica si ambos son del tipo correcto (int, float, etc.)
                // Por ejemplo, supongamos que nodo contiene el tipo real de la variable
                // Typecasting y verificación de tipos específicos (a ajustar según implementación)
                if ((int)simbolo1->nodo != TIPO_INT || (int)simbolo2->nodo != TIPO_INT) {
                    printf("Error: Operandos inválidos para '*'\n");
                    return -1;
                }
            }
            break;
        // Otros operadores pueden agregarse aquí
    }
    return 0;
}

// Validación de invocación de función
int validar_invocacion_funcion(NodoSimbolo *simbolo, int num_args) { //Esta función no la entiendo, hice validarInvocacionAFuncion() que anda bien, pero no borro esta por las dudas
    if (!simbolo || simbolo->tipo != FUNCION) {
        printf("Error: '%s' no es una función declarada.\n", simbolo->nombre);
        return -1;
    }
    // Validación específica para funciones con respecto a los argumentos
    // Tipo de datos de cada argumento a verificar en nodo de la función
    return 0;
}

// Validación de asignación
int validar_asignacion(NodoSimbolo *simbolo_lado_izq, NodoSimbolo *simbolo_lado_der) {
    if (!simbolo_lado_izq || !simbolo_lado_der) {
        printf("Error: Variable no declarada en la asignación.\n");
        return -1;
    }
    if (simbolo_lado_izq->tipo != VARIABLE || simbolo_lado_der->tipo != VARIABLE) {
        printf("Error: Asignación solo permitida entre variables.\n");
        return -1;
    }
    if (simbolo_lado_izq->nodo != simbolo_lado_der->nodo) {
        printf("Error: Tipos incompatibles en la asignación.\n");
        return -1;
    }
    return 0;
}

void validarInvocacionAFuncion(NodoErroresSemanticos **listaErroresSemanticos, char *identificador, char *listaDeArgumentos, int linea, int columna){
    NodoSimbolo *nodoPrevio = buscar_simbolo(identificador);
    if (nodoPrevio == NULL){
        char mensaje[256];
        snprintf(mensaje, sizeof(mensaje), " Funcion %s sin declarar", identificador);
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

int contarArgumentos(char *listaDeArgumentos) {
    int contador = 0;
    char *token;
    token = strtok(listaDeArgumentos, ",");
    while (token != NULL) {
        contador++;
        token = strtok(NULL, ",");
    }
    return contador;
}

/*NodoSimbolo *agregarSimboloTS (char const *sym_name, int sym_type){
  NodoSimbolo *ptr = (NodoSimbolo *) malloc (sizeof (NodoSimbolo));
  ptr->nombre = (char *) malloc (strlen (sym_name) + 1);
  strcpy (ptr->nombre,sym_name);
  ptr->tipo = sym_type;
  ptr->nodo.variable = 0;

    switch (sym_type)
        {
    case FUNCION:
      ptr->nodo.funcion = (NodoFuncion *) 0;
      break;

    case VARIABLE:
        ptr->nodo.variable = (NodoVariableDeclarada *) 0;
        break;

       }

  ptr->siguiente = (struct NodoSimbolo *)tablaSimbolos;
  tablaSimbolos = ptr;
  return ptr;
}



//Definición de la función getsym

NodoSimbolo *obtenerSimboloTS (char const *nombreSimbolo)
{
  NodoSimbolo *ptr;
  for (ptr = tablaSimbolos; ptr != (NodoSimbolo *) 0;
       ptr = (NodoSimbolo *)ptr->siguiente)
    if (strcmp (ptr->nombre, nombreSimbolo) == 0)
      return ptr;
  return 0;
}*/