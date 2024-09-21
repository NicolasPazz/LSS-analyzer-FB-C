#ifndef GENERAL_H
#define GENERAL_H
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
    char *tipoDato;
    struct NodoVariableDeclarada *siguiente;
} NodoVariableDeclarada;

NodoVariableDeclarada* crearNodoVariableDeclarada(const char *variableDeclarada);

void agregarVariableDeclarada(NodoVariableDeclarada **lista, const char *variableDeclarada, const char *tipoDato);

void imprimirVariablesDeclaradas(NodoVariableDeclarada *lista);

void liberarVariablesDeclaradas(NodoVariableDeclarada *lista);

extern NodoVariableDeclarada* listaVariablesDeclaradas;


// FUNCIONES
typedef struct NodoFuncion {
    char *funcion;
    struct NodoFuncion *siguiente;
} NodoFuncion;

NodoFuncion* crearNodoFuncion(const char *funcion);

void agregarFuncion(NodoFuncion **lista, const char *funcion);

void imprimirFunciones(NodoFuncion *lista);

void liberarFunciones(NodoFuncion *lista);

extern NodoFuncion* listaFunciones;


// SENTENCIAS
typedef struct NodoSentencia {
    char *sentencia;
    int linea;
    int columna;
    struct NodoSentencia *siguiente;
} NodoSentencia;

NodoSentencia* crearNodoSentencia(const char *sentencia, const int linea, const int columna);

void agregarSentencia(NodoSentencia **lista, const char *sentencia, const int linea, const int columna);
//void agregarSentencia(listaSentencias, sentencia, tipoSentencia, linea, columna);


void imprimirSentencias(NodoSentencia *lista);

void liberarSentencias(NodoSentencia *lista);

extern NodoSentencia* listaSentencias;


// ESTRUCTURAS NO RECONOCIDAS
typedef struct NodoEstructuraNoReconocida {
    char *estructuraNoReconocida;
    struct NodoEstructuraNoReconocida *siguiente;
} NodoEstructuraNoReconocida;

NodoEstructuraNoReconocida* crearNodoEstructuraNoReconocida(const char *estructuraNoReconocida);

void agregarEstructuraNoReconocida(NodoEstructuraNoReconocida **lista, const char *estructuraNoReconocida);

void imprimirEstructurasNoReconocidas(NodoEstructuraNoReconocida *lista);

void liberarEstructurasNoReconocidas(NodoEstructuraNoReconocida *lista);

extern NodoEstructuraNoReconocida* listaEstructurasNoReconocidas;


// CADENAS NO RECONOCIDAS
typedef struct NodoCadenaNoReconocida {
    char *cadenaNoReconocida;
    int linea;
    int columna;
    struct NodoCadenaNoReconocida *siguiente;
} NodoCadenaNoReconocida;

NodoCadenaNoReconocida* crearNodoCadenaNoReconocida(const char *cadenaNoReconocida, int linea, int columna);

void agregarCadenaNoReconocida(NodoCadenaNoReconocida **lista, const char *cadenaNoReconocida, int linea, int columna);

void imprimirCadenasNoReconocidas(NodoCadenaNoReconocida *lista);

void liberarCadenasNoReconocidas(NodoCadenaNoReconocida *lista);

extern NodoCadenaNoReconocida* listaCadenasNoReconocidas;


// FUNCIONES

char* copiarCadena(const char *str);

#endif