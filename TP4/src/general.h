#include <stddef.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef GENERAL_H
#define GENERAL_H
#define YYLTYPE YYLTYPE
typedef struct YYLTYPE{
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


// ESTRUCTURAS DE DATOS Y VARIABLES GLOBALES
    // SUFIJO
    typedef enum tipoDato{
        VACIO_TIPODATO,

        CHAR_TIPODATO,
        VOID_TIPODATO,
        DOUBLE_TIPODATO,
        FLOAT_TIPODATO,

        INT_TIPODATO,
        UNSIGNED_INT_TIPODATO,
        
        LONG_TIPODATO,             
        UNSIGNED_LONG_TIPODATO,    

        SHORT_TIPODATO,             
        UNSIGNED_SHORT_TIPODATO   
    }tipoDato;

    extern const char* especificadorTiposString[];
    typedef enum especificadorAlmacenamiento{
        AUTO_ESPALMAC,
        REGISTER_ESPALMAC,
        STATIC_ESPALMAC,
        EXTERN_ESPALMAC,
        TYPEDEF_ESPALMAC,
        VACIO_ESPALMAC
    }especificadorAlmacenamiento;

    extern const char* especificadorAlmacenamientoString[];

    typedef enum calificadorTipo{
        CONST_CALIFICADORTIPO,
        VOLATILE_CALIFICADORTIPO,
        VACIO_CALIFICADORTIPO
    }calificadorTipo;

    extern const char* calificadorTipoString[];

typedef enum expresionPrimaria{
    IDENTIFICADOR_EXPRESIONPRIMARIA,
    CONSTANTE_EXPRESIONPRIMARIA,
    VACIA_EXPRESIONPRIMARIA,
    CADENA_EXPRESIONPRIMARIA      
}expresionPrimaria;

extern expresionPrimaria tipoResultanteEvExpresion;

typedef struct ExpresionesTipo{
    expresionPrimaria tipoExpresion;
    
}ExpresionesTipo;
 
typedef struct EspecificadorTipos{
    tipoDato esTipoDato;
    especificadorAlmacenamiento esAlmacenamiento;
    calificadorTipo esCalificador;
}EspecificadorTipos;

extern EspecificadorTipos tipoRetorno;

EspecificadorTipos combinarEspecificadorTipos(EspecificadorTipos a, EspecificadorTipos b);

typedef struct IdentificadorTemporal {
    char *identificador;
    int linea;
    int columna;
    struct IdentificadorTemporal *siguiente;
} IdentificadorTemporal;

extern IdentificadorTemporal *listaTemporalIdentificadores;
extern char *contextoActual;

typedef enum tipoFuncion{
    DEFINICION_FUNCION,
    DECLARACION_FUNCION,
    OTRO
}tipoFuncion;

extern const char *tipoFuncionString[];


// TABLA DE SIMBOLOS
 typedef enum tipoSimbolo{
    VARIABLE,
    FUNCION
  }tipoSimbolo;

typedef struct NodoSimbolo{
  char *nombre;
  void *nodo;
  tipoSimbolo tipo;
  int linea;
  int columna;
  struct NodoSimbolo *siguiente; 
}NodoSimbolo;

extern NodoSimbolo* tablaSimbolos;

NodoSimbolo *crearNodoSimbolo(const char *nombre, tipoSimbolo tipo, int linea, int columna, void *nodo);

NodoSimbolo *buscar_simbolo(char *nombre);

void imprimirTablaSimbolos(NodoSimbolo *tablaSimbolos);


// CADENAS NO RECONOCIDAS - ERRORES LEXICOS
typedef struct NodoCadenaNoReconocida {
    char *cadenaNoReconocida;
    int linea;
    int columna;
    struct NodoCadenaNoReconocida *siguiente;
} NodoCadenaNoReconocida;

extern NodoCadenaNoReconocida* listaCadenasNoReconocidas;

NodoCadenaNoReconocida* crearNodoCadenaNoReconocida(const char *cadenaNoReconocida, int linea, int columna);

void agregarCadenaNoReconocida(NodoCadenaNoReconocida **lista, const char *cadenaNoReconocida, int linea, int columna);

void imprimirCadenasNoReconocidas(NodoCadenaNoReconocida *lista);

void liberarCadenasNoReconocidas(NodoCadenaNoReconocida *lista);


// ESTRUCTURAS NO RECONOCIDAS - ERRORES SINTACTICOS
typedef struct NodoErrorSintactico {
    char *errorSintactico;
    int linea;
    struct NodoErrorSintactico *siguiente;
} NodoErrorSintactico;

extern NodoErrorSintactico* listaErrorSintactico;
extern NodoErrorSintactico* listaSecuenciasLeidas;

NodoErrorSintactico* crearNodoErrorSintactico(const char *errorSintactico, const int linea);

void agregarErrorSintactico(NodoErrorSintactico **listaErroresSintacticos, NodoErrorSintactico **listaSecuenciasLeidas);

void eliminarEspacios(char *cadena);

void imprimirErrorSintactico(NodoErrorSintactico *lista);

void liberarErrorSintactico(NodoErrorSintactico *lista);

void concatenarLeido(NodoErrorSintactico **listaSecuenciasLeidas, const char *yytext, int linea);


// ERRORES SEMANTICOS 
typedef struct NodoErroresSemanticos {
    char *mensaje;
    int linea;
    int columna;
    struct NodoErroresSemanticos *siguiente;
} NodoErroresSemanticos;

extern NodoErroresSemanticos* listaErroresSemanticos;

NodoErroresSemanticos *crearNodoErroresSemanticos(const char *mensaje, const int linea, const int columna);

void agregarErrorSemantico(NodoErroresSemanticos **lista, const char *mensaje, const int linea, const int columna);

void imprimirErrorSemantico(NodoErroresSemanticos *lista);

/*void liberarErrorSemantico(NodoErroresSemanticos *lista);*/

bool esMultiplicable(char *expresion);

char *extraerTipoDato(char *expresion);

const char *extraerTipoDato2(char *expresion);

void validarMultiplicacion(char *expresion1, char *expresion2, int linea, int columna, NodoErroresSemanticos **listaErroresSemanticos);


// PARAMETROS
typedef struct Parametro {
    EspecificadorTipos especificadorDeclaracion;
    char *identificador;
    int linea;
    int columna;
    struct Parametro *siguiente;
} Parametro;

extern Parametro* listaDeParametros;
extern Parametro* listaDeParametrosInvocacion;

typedef struct ElementosParametro{
    char * elemento;
    struct ElementosParametro *siguiente;
}ElementosParametro;

Parametro *crearNodoParametro(EspecificadorTipos especificadorDeclaracion, const char *identificador, int linea, int columna);

int contarArgumentos(Parametro *listaDeParametros);

void agregarParametro(Parametro **listaDeParametros, EspecificadorTipos especificadorDeclaracion, char *identificador, int linea, int columna);

char* imprimirParametros(Parametro *listaDeParametros);

int validarListasDeParametros(Parametro* listaDeParametrosEncontrados, Parametro* listaDeParametrosInvocados);

char *imprimirParametrosSinIdentificador(Parametro *listaDeParametros);


// FUNCIONES
typedef struct NodoFuncion {
    EspecificadorTipos retorno;
    Parametro *listaDeParametros;
    tipoFuncion tipogramatica;
    struct NodoFuncion *siguiente;
} NodoFuncion;

extern NodoSimbolo* listaFunciones;
extern NodoSimbolo* nodoGenericoFuncion;

NodoFuncion *crearNodoFuncion(Parametro *listaDeParametros, EspecificadorTipos retorno, tipoFuncion tipogramatica);

void agregarFuncion(NodoSimbolo **lista, NodoSimbolo **tablaSimbolos, EspecificadorTipos retorno, NodoSimbolo**nodoGenericoFuncion, const int linea, tipoFuncion tipogramatica, const int columna);

NodoSimbolo* fueDefinidaAntes(NodoSimbolo *tablaSimbolos, char* nombre);

void imprimirFunciones(NodoSimbolo *lista);

/*void liberarFunciones(NodoFuncion *lista);*/

void validarInvocacionAFuncion(NodoErroresSemanticos **listaErroresSemanticos, char *identificador, Parametro *listaDeParametros, int linea, int columna, int cantidadDeParametros);

void llenarNodoGenericoFuncion(NodoSimbolo **nodoGenericoFuncion, const char *identificador, Parametro **listaDeParametros);

char *asignarTipoDatoFuncion(char* identificador);

void ValidarInicializacionVoid(char* tipoDato, int linea, int columna);


// VARIABLES DECLARADAS
typedef struct NodoVariableDeclarada {
  EspecificadorTipos  tipoDato;    
} NodoVariableDeclarada;

extern NodoSimbolo* listaVariablesDeclaradas;

NodoVariableDeclarada *crearNodoVariableDeclarada(EspecificadorTipos tipoDato);

void agregarVariableDeclarada(NodoSimbolo **tablaSimbolos, NodoErroresSemanticos **listaErroresSemanticos, char *identificador, EspecificadorTipos tipoDato, int linea, int columna);

void imprimirVariables(NodoSimbolo *lista);

/*void liberarVariablesDeclaradas(NodoVariableDeclarada *lista);*/

void agregarIdentificadorTemporal(IdentificadorTemporal **listaTemporalIdentificadores, char *identificador, int linea, int columna);

void agregarListaVariables(IdentificadorTemporal *listaTemporalIdentificadores, EspecificadorTipos tipoDato);

void validarUsoDeVariable(NodoErroresSemanticos **listaErroresSemanticos, char *identificador, char *contextoActual, int linea, int columna, IdentificadorTemporal* listaTemporalIdentificadores);

const char *enumAString1(EspecificadorTipos tipoDato);

char *enumAString2(EspecificadorTipos tipoDato);


// FUNCIONES AUXILIARES DE UTILIDAD
char* copiarCadena(const char *str);



/*-------------------------------------------------------------------------------------------------------------------*/
//funciones de validacion de asignacion
// bool esValorModificable(NodoSimbolo *simbolo);
// void validarAsignacion(NodoErroresSemanticos **listaErroresSemanticos, NodoSimbolo *simboloLadoIzq,  EspecificadorTipos tipoLadoDer, bool esValorModificable, int linea, int columna) ;

//funciones de validaciones de return
//void validarSentenciaReturn(NodoErroresSemanticos **listaErroresSemanticos,  EspecificadorTipos tipoRetornoDeclarado, EspecificadorTipos tipoReturnSentencia,  int lineaReturn, int columnaReturn, int lineaDeclaracion, int columnaDeclaracion);

#endif