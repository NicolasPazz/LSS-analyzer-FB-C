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

typedef struct NodoErroresSemanticos {
    char *mensaje;
    int linea;
    int columna;
    struct NodoSentencia *siguiente;
} NodoErroresSemanticos;

#define INICIO_CONTEO_LINEA 1
#define INICIO_CONTEO_COLUMNA 1

void pausa(void);
void inicializarUbicacion(void);
void reinicializarUbicacion(void);


// Tabla de símbolos
 typedef enum tipoSimbolo{
    VARIABLE,
    FUNCION
  }tipoSimbolo;

typedef struct NodoSimbolo{
  char *nombre;
  enum tipoSimbolo tipo;
  void* nodo;
  struct NodoSimbolo *siguiente; //Puntero al siguiente nodo de la lista
}NodoSimbolo;

extern NodoSimbolo* tablaSimbolos;

NodoSimbolo* buscarSimbolo(const char *nombre);

//NodoSimbolo *agregarSimboloTS (char const *, int);
//NodoSimbolo *obtenerSimboloTS (char const *);



//TODO:: hago un switch para saber si es variable o funcion typecast


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

void agregarVariableDeclarada(NodoVariableDeclarada **lista, NodoSimbolo **tablaSimbolos, NodoErroresSemanticos **listaErroresSemanticos, const char *variableDeclarada, const char *tipoDato, const int linea, const int columna, const char *sufijo);

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
    int columna;
    struct NodoFuncion *siguiente;
} NodoFuncion;

NodoFuncion* crearNodoFuncion(const char *retorno, const char *funcion, const int linea, const char* tipogramatica);

void agregarFuncion(NodoFuncion **lista, NodoSimbolo **tablaSimbolos, const char *retorno, const char *funcion, const int linea, const char* tipogramatica, const int columna);

void agregarParametro(char** lista, char* parametro);

char* unirParametros(const char* param1, const char* param2);

void imprimirFunciones(NodoFuncion *lista);

void liberarFunciones(NodoFuncion *lista);

extern NodoFuncion* listaFunciones;

extern char* listaParametros;

extern char* parametro;

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

NodoSimbolo* crearNodoSimbolo(const char *nombre, tipoSimbolo tipo, void* nodo);

extern NodoCadenaNoReconocida* listaCadenasNoReconocidas;
//void reiniciarListaParametros(char **listaParametros);


//ERRORES SEMANTICOS 

NodoErroresSemanticos* crearNodoErrorSemantico(const char *mensaje, const int linea, const int columna);

void agregarErrorSemantico(NodoErroresSemanticos **lista, const char *mensaje, const int linea, const int columna);

void imprimirErrorSemantico(NodoErroresSemanticos *lista);

void liberarErrorSemantico(NodoErroresSemanticos *lista);

extern NodoErroresSemanticos* listaErroresSemanticos;


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


//NodoSimbolo *agregarSimboloTS (char const *sym_name, int sym_type);
//NodoSimbolo *obtenerSimboloTS (char const *sym_name);

int validar_invocacion_funcion(NodoSimbolo *simbolo, int num_args);
int validar_asignacion(NodoSimbolo *simbolo_lado_izq, NodoSimbolo *simbolo_lado_der);
int validar_operacion(NodoSimbolo *simbolo1, NodoSimbolo *simbolo2, char operador) ;
int insertar_simbolo(const char *nombre, tipoSimbolo tipo, void *nodo) ;
NodoSimbolo *buscar_simbolo(char *nombre);
#endif

