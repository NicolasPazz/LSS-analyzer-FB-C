#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
    struct NodoErroresSemanticos *siguiente;
} NodoErroresSemanticos;

#define INICIO_CONTEO_LINEA 1
#define INICIO_CONTEO_COLUMNA 1

void pausa(void);
void inicializarUbicacion(void);
void reinicializarUbicacion(void);


// Declaracion de enum para armar un arbol de identificacion de tipos de datos. Sirve para la comparativa del return con la funcion.
 typedef enum tipoDato{
    INT_TIPODATO,
    DOUBLE_TIPODATO,
    FLOAT_TIPODATO,
    CHAR_TIPODATO,
    VOID_TIPODATO,
    VACIO_TIPODATO
  }tipoDato;

   typedef enum especificadorAlmacenamiento{
    AUTO_ESPALMAC ,
    REGISTER_ESPALMAC,
    STATIC_ESPALMAC,
    EXTERN_ESPALMAC,
    TYPEDEF_ESPALMAC
    VACIO_ESPALMAC
  }especificadorAlmacenamiento;

 typedef enum calificadorTipo{
    CONST_CALIFICADORTIPO,
    VOLATILE_CALIFICADORTIPO
    VACIO_CALIFICADORTIPO
 }calificadorTipo;

 typedef enum expresionPrimaria{
    IDENTIFICADOR_EXPRESIONPRIMARIA,
    CONSTANTE_EXPRESIONPRIMARIA,
    VACIA_EXPRESIONPRIMARIA, //NULL EL return
    CADENA_EXPRESIONPRIMARIA      
 }expresionPrimaria;

 extern expresionPrimaria tipoResultanteEvExpresion;
 
 //desarrolamos ya identificamos que es tipo IDENTIFICADOR
 //con ese IDENTIFICADOR Vamos a la tabla de simbolos

// Tabla de simbolos
 typedef enum tipoSimbolo{
    VARIABLE,
    FUNCION
  }tipoSimbolo;

typedef struct NodoSimbolo{
  char *nombre;
  void* nodo;
  struct NodoSimbolo *siguiente; //Puntero al siguiente nodo de la lista
}NodoSimbolo;

extern NodoSimbolo* tablaSimbolos;


// VARIABLES DECLARADAS
typedef struct NodoVariableDeclarada {
    char *variableDeclarada;
    enum tipoDato tipoDato;
    int linea;
    int columna;
    char *sufijo;
    struct NodoVariableDeclarada *siguiente;
} NodoVariableDeclarada;

NodoVariableDeclarada* crearNodoVariableDeclarada(const char *variableDeclarada, const char *tipoDato, const int linea, const int columna, const char *sufijo);

void agregarVariableDeclarada(NodoVariableDeclarada **lista, NodoSimbolo **tablaSimbolos, NodoErroresSemanticos **listaErroresSemanticos, char *variableDeclarada, const char *tipoDato, const int linea, const int columna, const char *sufijo);

void imprimirVariablesDeclaradas(NodoVariableDeclarada *lista);

void liberarVariablesDeclaradas(NodoVariableDeclarada *lista);

extern NodoVariableDeclarada* listaVariablesDeclaradas;


// FUNCIONES


typedef struct Parametro {
    char *sufijo;
    char *tipo;
    char *identificador;
    struct Parametro *siguiente;
} Parametro;
typedef struct NodoFuncion {
    char *funcion;
    enum  tipoDato retorno;
    Parametro *listaDeParametros;
    char *tipogramatica;
    int linea;
    int columna;
    struct NodoFuncion *siguiente;
} NodoFuncion;


NodoFuncion* crearNodoFuncion(Parametro* listaDeParametros, const char *retorno, const char *funcion, int linea, const char* tipogramatica);

void agregarFuncion(NodoFuncion **lista, NodoSimbolo **tablaSimbolos, const char *retorno, NodoFuncion **nodoGenericoFuncion, const int linea, const char* tipogramatica, const int columna);
bool noFueDefinidaAntes(NodoSimbolo *tablaSimbolos, NodoFuncion *nodoGenericoFuncion);
void agregarParametro(Parametro **listaDeParametros, char* sufijo, char* tipo, char* identificador);

char* unirParametros(const char* param1, const char* param2);

void imprimirFunciones(NodoFuncion *lista);
char* imprimirParametros(Parametro *listaDeParametros);
void liberarFunciones(NodoFuncion *lista);

extern NodoFuncion* listaFunciones;

extern Parametro* listaDeParametros;

extern NodoFuncion* nodoGenericoFuncion;

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

extern char* copiarCadena(const char *str);

//Rutinas

// Define los tipos de datos posibles
/*typedef enum {
    TIPO_INT,
    TIPO_FLOAT,
    TIPO_ERROR
     // Para manejar errores de tipo
} Type;

// Declara la funcion check_type para usar en el control de tipos
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

// Declara la funcion check_type para usar en el control de tipos
Type check_type(char *left, char *right, const int linea, const int columna);

//estructura para saber tipo de dato en cada token
typedef struct{
    char tipo[20];
    union{
        int numero;
        char cadena[100];
    } valor;
}TokenInfo;

typedef struct {
    char *tipoDato; // Tipo de dato de la expresión return
    int linea;
    int columna;
   struct TipoRetorno *siguiente;
} TipoRetorno;

// Variable global para almacenar el tipo de retorno esperado de la función
extern char *tipoReturnEsperado;
extern TipoRetorno *tipoReturnEncontrado;


void concatenarLeido(NodoErrorSintactico **listaSecuenciasLeidas, const char *yytext, int linea);


//NodoSimbolo *agregarSimboloTS (char const *sym_name, int sym_type);
//NodoSimbolo *obtenerSimboloTS (char const *sym_name);

int validar_invocacion_funcion(NodoSimbolo *simbolo, int num_args);
int validar_asignacion(NodoSimbolo *simbolo_lado_izq, NodoSimbolo *simbolo_lado_der);
int validar_operacion(NodoSimbolo *simbolo1, NodoSimbolo *simbolo2, char operador) ;
int insertar_simbolo(char *nombre, tipoSimbolo tipo, void *nodo) ;
NodoSimbolo *buscar_simbolo(char *nombre);
void validarInvocacionAFuncion(NodoErroresSemanticos **listaErroresSemanticos, char *identificador, char *listaDeArgumentos, int linea, int columna);
Parametro* crearNodoParametro(char* sufijo, char* tipo, char* identificador);
void agregarParametro(Parametro **listaDeParametros, char* sufijo, char *tipo, char *identificador);
void llenarNodoGenericoFuncion(NodoFuncion **nodoGenericoFuncion, char *identificador, Parametro **listaDeParametros);
void imprimirTablaSimbolos(NodoSimbolo *tablaSimbolos);

void inicializarTipoRetorno(const char *tipo) ;
void registrarReturn(const char *tipo, int linea, int columna);
void validarTipoReturn(NodoErroresSemanticos **listaErroresSemanticos);

char* obtenerTipoIdentificador(const char *identificador) ;
#endif

