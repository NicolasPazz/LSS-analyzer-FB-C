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


// Tabla de símbolos
 typedef enum tipoSimbolo
  {
    VARIABLE,
    FUNCION
  } tipoSimbolo;

struct NodoSimbolo
{
  char *nombre;
  enum tipoSimbolo tipoSimbolo; 
  void* nodo;  
  struct NodoSimbolo *next; //Puntero al siguiente nodo de la lista
};

//TODO:: hago un switch para saber si es variable o funcion typecast

//extern NodoSimbolo* tablaDeSimbolos;

//NodoSimbolo* crearNodoSimboloVariable(const char *nombre, const char *tipo, NodoVariableDeclarada variable);
//NodoSimbolo* crearNodoSimboloFuncion(const char *nombre, const char *tipo, NodoFuncion funcion);

// VARIABLES DECLARADAS
typedef struct NodoVariableDeclarada {
    char *variableDeclarada;
    char *tipoDato;
    int linea;
    char *sufijo;
    struct NodoVariableDeclarada *siguiente;
} NodoVariableDeclarada;

NodoVariableDeclarada* crearNodoVariableDeclarada(const char *variableDeclarada, const char *tipoDato, const int linea, const char *sufijo);

void agregarVariableDeclarada(NodoVariableDeclarada **lista, const char *variableDeclarada, const char *tipoDato, const int linea, const char *sufijo);

void imprimirVariablesDeclaradas(NodoVariableDeclarada *lista);

void liberarVariablesDeclaradas(NodoVariableDeclarada *lista);

extern NodoVariableDeclarada* listaVariablesDeclaradas;


// FUNCIONES
typedef struct NodoFuncion {
    char *funcion;
    char *retorno;
    char *parametros;
    char *tipogramatica;
    int linea;
    struct NodoFuncion *siguiente;
} NodoFuncion;

NodoFuncion* crearNodoFuncion(const char *retorno, const char *funcion, const int linea, const char* tipogramatica);

void agregarFuncion(NodoFuncion **lista, const char *retorno, const char *funcion, const int linea, const char* tipogramatica);

void agregarParametro(char** lista, char* parametro);

char* unirParametros(const char* param1, const char* param2);

void imprimirFunciones(NodoFuncion *lista);

void liberarFunciones(NodoFuncion *lista);

extern NodoFuncion* listaFunciones;

extern char* listaParametros;

extern char* parametro;

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
    int linea;
    struct NodoEstructuraNoReconocida *siguiente;
} NodoEstructuraNoReconocida;

NodoEstructuraNoReconocida* crearNodoEstructuraNoReconocida(const char *estructuraNoReconocida, int linea);

void agregarEstructuraNoReconocida(NodoEstructuraNoReconocida **lista, const char *estructuraNoReconocida, int linea);

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