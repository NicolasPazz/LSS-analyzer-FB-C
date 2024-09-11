/* Inicio de la seccion de prólogo (declaraciones y definiciones de C y directivas del preprocesador) */
%{
#include <stdio.h>
#include <math.h>

#include "general.h"

	/* Declaración de la funcion yylex del analizador léxico, necesaria para que la funcion yyparse del analizador sintáctico pueda invocarla cada vez que solicite un nuevo token */
extern int yylex(void);
	/* Declaracion de la función yyerror para reportar errores, necesaria para que la función yyparse del analizador sintáctico pueda invocarla para reportar un error */
void yyerror(const char*);
%}
/* Fin de la sección de prólogo (declaraciones y definiciones de C y directivas del preprocesador) */

/* Inicio de la sección de declaraciones de Bison */

	/* Para requerir una versión mínima de Bison para procesar la gramática */
/* %require "2.4.1" */

	/* Para requirle a Bison que describa más detalladamente los mensajes de error al invocar a yyerror */
%error-verbose
	/* Nota: esta directiva (escrita de esta manera) quedó obsoleta a partir de Bison v3.0, siendo reemplazada por la directiva: %define parse.error verbose */

	/* Para activar el seguimiento de las ubicaciones de los tokens (número de linea, número de columna) */
%locations

	/* Para especificar la colección completa de posibles tipos de datos para los valores semánticos */
%union {
    char cadena[30];
    int entero;
}

        /* */
%token IDENTIFICADOR
%token <entero> OCTAL
%token <entero> HEXADECIMAL
%token <entero> DECIMAL
%token CARACTER
%token LITERALCADENA
%token MASMASOMENOSMENOS
%token MULTIPLICATIVAS
%token ADITIVAS
%token RELACIONALES
%token DEIGUALDAD
%token DEASIGNACION
%token TIPODEDATO

	/* */
%type sentenciaif
%type sentenciaifelse
%type sentenciaswitch
%type sentenciawhile
%type sentenciadowhile
%type sentenciafor
%type sentencia
%type sentencias
%type sentenciacompuesta
%type expresion
%type expresionentreparentesis
%type case
%type cases
%type continue
%type break
%type return
%type default

	/* Para especificar el no-terminal de inicio de la gramática (el axioma). Si esto se omitiera, se asumiría que es el no-terminal de la primera regla */
%start input

/* Fin de la sección de declaraciones de Bison */

/* Inicio de la sección de reglas gramaticales */
%%

sentenciaif:
    "if" expresionentreparentesis sentenciacompuesta
    ;

sentenciaifelse:
    "if" expresionentreparentesis sentenciacompuesta
    "else" sentenciacompuesta
    ;

sentenciaswitch:
    "switch" expresionentreparentesis '{' cases '}'
    | "switch" expresionentreparentesis '{' cases default '}' //fijarme si el default puede ir en otra parte que no sea el final 
    ;

sentenciawhile:
    "while" expresionentreparentesis sentenciacompuesta
    ;

sentenciadowhile:
    "do" sentenciacompuesta
    "while" expresionentreparentesis ';'
    ;

sentenciafor:
    "for" expresionentreparentesis sentenciacompuesta //modificar, no puede ser cualquier expresión entre paréntesis
    ;

sentencia:
    //posibles sentencias
    ;

sentencias:
    sentencia
    | sentencias sentencia
    ;

sentenciacompuesta:
    '{' sentencias '}'
    ;

expresion:
    //posibles expresiones
    | expresionentreparentesis
    ;

expresionentreparentesis:
    '(' expresion ')'
    ;

case:
    "case" expresion ':' sentencias
    ;

cases:
    case
    | cases case
    ;

continue:
    "continue;"
    ;

break:
    "break;"
    ;

return:
    "return" expresion ';'
    ;

default:
    "default:"
    ;

%%
/* Fin de la sección de reglas gramaticales */

/* Inicio de la sección de epílogo (código de usuario) */

int main(void)
{
        inicializarUbicacion();

        #if YYDEBUG
                yydebug = 1;
        #endif

        while(1)
        {
                printf("Ingrese una expresion aritmetica en notacion polaca inversa para resolver:\n");
                printf("(La funcion yyparse ha retornado con valor: %d)\n\n", yyparse());
                /* Valor | Significado */
                /*   0   | Análisis sintáctico exitoso (debido a un fin de entrada (EOF) indicado por el analizador léxico (yylex), ó bien a una invocación de la macro YYACCEPT) */
                /*   1   | Fallo en el análisis sintáctico (debido a un error en el análisis sintáctico del que no se pudo recuperar, ó bien a una invocación de la macro YYABORT) */
                /*   2   | Fallo en el análisis sintáctico (debido a un agotamiento de memoria) */
        }

        pausa();
        return 0;
}

	/* Definición de la funcion yyerror para reportar errores, necesaria para que la funcion yyparse del analizador sintáctico pueda invocarla para reportar un error */
void yyerror(const char* literalCadena)
{
        fprintf(stderr, "Bison: %d:%d: %s\n", yylloc.first_line, yylloc.first_column, literalCadena);
}

/* Fin de la sección de epílogo (código de usuario) */