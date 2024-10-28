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

typedef struct NodoSimbolo
{
  char *nombre;
  enum tipoSimbolo tipo;
  void* nodo;
  struct NodoSimbolo *siguiente; //Puntero al siguiente nodo de la lista
} NodoSimbolo;

extern NodoSimbolo* tablaSimbolos;

//TODO:: hago un switch para saber si es variable o funcion typecast

NodoSimbolo* crearNodoSimbolo(const char *nombre, tipoSimbolo tipo, void* nodo);

// VARIABLES DECLARADAS
typedef struct NodoVariableDeclarada {
    char *variableDeclarada;
    char *tipoDato;
    int linea;
    int columna;
    char *sufijo;
    struct NodoVariableDeclarada *siguiente;
} NodoVariableDeclarada;

NodoVariableDeclarada* crearNodoVariableDeclarada(const char *variableDeclarada, const char *tipoDato, const int linea, const int columna, const char *sufijo);

void agregarVariableDeclarada(NodoVariableDeclarada **lista, NodoSimbolo **tablaSimbolos, const char *variableDeclarada, const char *tipoDato, const int linea, const int columna, const char *sufijo);

void imprimirVariablesDeclaradas(NodoVariableDeclarada *lista);

void liberarVariablesDeclaradas(NodoVariableDeclarada *lista);

extern NodoVariableDeclarada* listaVariablesDeclaradas;


// FUNCIONES
typedef struct NodoFuncion {
    char *funcion;
    char *retorno;
    char *parametro;
    char *tipogramatica;
    int linea;
    struct NodoFuncion *siguiente;
} NodoFuncion;

NodoFuncion* crearNodoFuncion(const char *retorno, const char *funcion, const int linea, const char* tipogramatica);

void agregarFuncion(NodoFuncion **lista, NodoSimbolo **tablaSimbolos, const char *retorno, const char *funcion, const int linea, const char* tipogramatica);

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
typedef struct NodoErrorSintactico {
    char *errorSintactico;
    int linea;
    struct NodoErrorSintactico *siguiente;
} NodoErrorSintactico;

NodoErrorSintactico* crearNodoErrorSintactico(const char *errorSintactico, const int linea);

void agregarErrorSintactico(NodoErrorSintactico **listaErroresSintacticos, NodoErrorSintactico **listaSecuenciasLeidas);

void eliminarEspacios(char *cadena);

void imprimirErrorSintactico(NodoErrorSintactico *lista);

void liberarErrorSintactico(NodoErrorSintactico *lista);

extern NodoErrorSintactico* listaErrorSintactico;

extern NodoErrorSintactico* listaSecuenciasLeidas;

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
void reiniciarListaParametros(char **listaParametros);

//ERRORES SEMANTICOS 
typedef struct NodoErrorSemantico {
    char *mensaje;
    int linea;
    int columna;
    struct NodoSentencia *siguiente;
} NodoErrorSemantico;

NodoErrorSemantico* crearNodoErrorSemantico(const char *mensaje, const int linea, const int columna);

void agregarErrorSemantico(NodoErrorSemantico **lista, const char *mensaje, const int linea, const int columna);
//void agregarSentencia(listaSentencias, sentencia, tipoSentencia, linea, columna);

void imprimirErrorSemantico(NodoErrorSemantico *lista);

void liberarErrorSemantico(NodoErrorSemantico *lista);

extern NodoErrorSemantico* listaErrorSemantico;


// FUNCIONES

char* copiarCadena(const char *str);

//Rutinas

// Define los tipos de datos posibles
/*typedef enum {
    TIPO_INT,
    TIPO_FLOAT,
    TIPO_ERROR
     // Para manejar errores de tipo
} Type;

// Declara la función check_type para usar en el control de tipos
Type check_type(char *left, char *right, const int linea, const int columna);

//estructura para saber tipo de dato en cada token
typedef struct{
    char tipo[20];
    union{
        int numero;
        char cadena[100];
    } valor;
}TokenInfo;*/

//Rutinas

// Define los tipos de datos posibles
typedef enum {
    TIPO_INT,
    TIPO_FLOAT,
    TIPO_ERROR
     // Para manejar errores de tipo
} Type;

// Declara la función check_type para usar en el control de tipos
Type check_type(char *left, char *right, const int linea, const int columna);

//estructura para saber tipo de dato en cada token
typedef struct{
    char tipo[20];
    union{
        int numero;
        char cadena[100];
    } valor;
}TokenInfo;

void concatenarLeido(NodoErrorSintactico **listaSecuenciasLeidas, const char *yytext, int linea);

#endif