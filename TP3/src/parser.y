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
    char* cadena;
}

        /* */

%token ENTERO

%token IF
%token ELSE
%token SWITCH
%token WHILE
%token DO
%token FOR
%token ABROPARENTESIS
%token CIERROPARENTESIS
%token ABROLLAVE
%token CIERROLLAVE
%token DOSPUNTOS
%token PUNTOYCOMA
%token CASE
%token CONTINUE
%token BREAK
%token RETURN
%token DEFAULT

%token <cadena> IDENTIFICADOR
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

	/* Para especificar el no-terminal de inicio de la gramática (el axioma). Si esto se omitiera, se asumiría que es el no-terminal de la primera regla */
%start input

/* Fin de la sección de declaraciones de Bison */

/* Inicio de la sección de reglas gramaticales */
%%

input:

    | input sentenciaif
    | input sentenciaifelse
    | input sentenciaswitch
    | input sentenciawhile
    | input sentenciadowhile
    | input sentenciafor
    | input sentenciacompuesta
    | input expresionentreparentesis
    | input case
    | input continue
    | input break
    | input return
    | input default
    | input error { yyerror("Error de sintaxis, avanzando..."); yyclearin; yyerrok; }
    ;

sentenciaif:
    IF expresionentreparentesis sentenciacompuesta {fprintf(output_file, "sentenciaif\n");}
    ;

sentenciaifelse:
    IF expresionentreparentesis sentenciacompuesta
    ELSE sentenciacompuesta {fprintf(output_file, "sentenciaifelse\n");}
    ;

sentenciaswitch:
    SWITCH expresionentreparentesis ABROLLAVE cases CIERROLLAVE {fprintf(output_file, "sentenciaswitch\n");}
    | SWITCH expresionentreparentesis ABROLLAVE cases default CIERROLLAVE {fprintf(output_file, "sentenciaifelse\n");}
    //fijarme si el default puede ir en otra parte que no sea el final 
    ;

sentenciawhile:
    WHILE expresionentreparentesis sentenciacompuesta {fprintf(output_file, "sentenciawhile\n");}
    ;

sentenciadowhile:
    DO sentenciacompuesta
    WHILE expresionentreparentesis PUNTOYCOMA {fprintf(output_file, "sentenciadowhile\n");}
    ;

sentenciafor:
    FOR expresionentreparentesis sentenciacompuesta {fprintf(output_file, "sentenciafor\n");}
    //modificar, no puede ser cualquier expresión entre paréntesis
    ;

sentencia:
    ENTERO
    ;

sentencias:
    sentencia
    | sentencias sentencia
    ;

sentenciacompuesta:
    ABROLLAVE sentencias CIERROLLAVE {fprintf(output_file, "sentenciacompuesta\n");}
    ;

expresion:
    ENTERO
    | expresionentreparentesis
    ;

expresionentreparentesis:
    ABROPARENTESIS expresion CIERROPARENTESIS {fprintf(output_file, "expresionentreparentesis\n");}
    ;

case:
    CASE expresion DOSPUNTOS sentencias {fprintf(output_file, "case\n");}
    ;

cases:
    case
    | cases case
    ;

continue:
    CONTINUE {fprintf(output_file, "continue\n");}
    ;

break:
    BREAK {fprintf(output_file, "break\n");}
    ;

return:
    RETURN expresion PUNTOYCOMA {fprintf(output_file, "return\n");}
    ;

default:
    DEFAULT {fprintf(output_file, "default\n");}
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