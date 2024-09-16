#ifndef GENERAL_H
#define GENERAL_H

/* En los archivos de cabecera (header files) (*.h) poner DECLARACIONES (evitar DEFINICIONES) de C, así como directivas de preprocesador */
/* Recordar solamente indicar archivos *.h en las directivas de preprocesador #include, nunca archivos *.c */

#define YYLTYPE YYLTYPE

typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;

#define INICIO_CONTEO_LINEA 1
#define INICIO_CONTEO_COLUMNA 1

void pausa(void);
void inicializarUbicacion(void);
void reinicializarUbicacion(void);


// VARIABLES DECLARADAS
typedef struct NodoVariableDeclarada {
    char *variableDeclarada;
    struct NodoVariableDeclarada *siguiente;
} NodoVariableDeclarada;

NodoVariableDeclarada* crearNodoVariableDeclarada(const char *variableDeclarada);

void agregarVariableDeclarada(NodoVariableDeclarada **lista, const char *variableDeclarada);

void imprimirVariablesDeclaradas(NodoVariableDeclarada *lista);

void liberarVariablesDeclaradas(NodoVariableDeclarada *lista);

NodoVariableDeclarada* listaVariablesDeclaradas = NULL;


// FUNCIONES
typedef struct NodoFuncion {
    char *funcion;
    struct NodoFuncion *siguiente;
} NodoFuncion;

NodoFuncion* crearNodoFuncion(const char *funcion);

void agregarFuncion(NodoFuncion **lista, const char *funcion);

void imprimirFunciones(NodoFuncion *lista);

void liberarFunciones(NodoFuncion *lista);

NodoFuncion* listaFunciones = NULL;


// SENTENCIAS
typedef struct NodoSentencia {
    char *sentencia;
    struct NodoSentencia *siguiente;
} NodoSentencia;

NodoSentencia* crearNodoSentencia(const char *sentencia);

void agregarSentencia(NodoSentencia **lista, const char *sentencia);

void imprimirSentencias(NodoSentencia *lista);

void liberarSentencias(NodoSentencia *lista);

NodoSentencia* listaSentencias = NULL;


// ESTRUCTURAS NO RECONOCIDAS
typedef struct NodoEstructuraNoReconocida {
    char *estructuraNoReconocida;
    struct NodoEstructuraNoReconocida *siguiente;
} NodoEstructuraNoReconocida;

NodoEstructuraNoReconocida* crearNodoEstructuraNoReconocida(const char *estructuraNoReconocida);

void agregarEstructuraNoReconocida(NodoEstructuraNoReconocida **lista, const char *estructuraNoReconocida);

void imprimirEstructurasNoReconocidas(NodoEstructuraNoReconocida *lista);

void liberarEstructurasNoReconocidas(NodoEstructuraNoReconocida *lista);

NodoEstructuraNoReconocida* listaEstructurasNoReconocidas = NULL;


// CADENAS NO RECONOCIDAS
typedef struct NodoCadenaNoReconocida {
    char *cadenaNoReconocida;
    struct NodoCadenaNoReconocida *siguiente;
} NodoCadenaNoReconocida;

NodoCadenaNoReconocida* crearNodoCadenaNoReconocida(const char *cadenaNoReconocida);

void agregarCadenaNoReconocida(NodoCadenaNoReconocida **lista, const char *cadenaNoReconocida);

void imprimirCadenasNoReconocidas(NodoCadenaNoReconocida *lista);

void liberarCadenasNoReconocidas(NodoCadenaNoReconocida *lista);

NodoCadenaNoReconocida* listaCadenasNoReconocidas = NULL;


// FUNCIONES

char* copiarCadena(const char *str);

#endif