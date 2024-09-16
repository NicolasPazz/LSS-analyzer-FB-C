/* Inicio de la seccion de prólogo (declaraciones y definiciones de C y directivas del preprocesador) */
%{
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "general.h"

	/* Declaración de la funcion yylex del analizador léxico, necesaria para que la funcion yyparse del analizador sintáctico pueda invocarla cada vez que solicite un nuevo token */
extern int yylex(void);
	/* Declaracion de la función yyerror para reportar errores, necesaria para que la función yyparse del analizador sintáctico pueda invocarla para reportar un error */
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
%}
/* Fin de la sección de prólogo (declaraciones y definiciones de C y directivas del preprocesador) */

/* Inicio de la sección de declaraciones de Bison */
%define parse.error verbose
	/* Para activar el seguimiento de las ubicaciones de los tokens (número de linea, número de columna) */
%locations


	/* Para especificar la colección completa de posibles tipos de datos para los valores semánticos */
%union {
    char* cadena;
    int entero;
}

%token <entero> ENTERO
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
%token TEXTO

%type 

%left '+' '-'
%left '*' '/'
%left '^'
%left '(' ')'

	/* Para especificar el no-terminal de inicio de la gramática (el axioma). Si esto se omitiera, se asumiría que es el no-terminal de la primera regla */
%start input

/* Fin de la sección de declaraciones de Bison */

/* Inicio de la sección de reglas gramaticales */
%%

input:
     /* produccion nula */
    | input line
    ;

line: 
      '\n'
    | expresion '\n'
    | sentencia '\n'
    | error '\n' { yyerror("Error de sintaxis, avanzando..."); yyclearin; yyerrok; }
    ;

expresion:
    


expresionentreparentesis:
    ABROPARENTESIS expresion CIERROPARENTESIS {fprintf(output_file, "expresionentreparentesis\n");}
    ;

sentencia:
    | sentenciadeexpresion '\n'
    | sentenciacompuesta '\n'

    | sentenciaif '\n'
    | sentenciaifelse '\n'
    | sentenciaswitch '\n'

    | sentenciawhile '\n'
    | sentenciadowhile '\n'
    | sentenciafor '\n'

    | continue '\n'
    | break '\n'
    | return '\n'
    ;

sentenciadeexpresion:
    expresion PUNTOYCOMA
    ;

sentenciacompuesta:
    ABROLLAVE sentencias CIERROLLAVE {fprintf(output_file, "sentenciacompuesta\n");}
    ;

sentencias:
    sentencia
    | sentencias sentencia
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

default:
    DEFAULT {fprintf(output_file, "default\n");}
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

%%
/* Fin de la sección de reglas gramaticales */

/* Inicio de la sección de epílogo (código de usuario) */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s archivoAAnalizar.i\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("No se puede abrir el archivo");
        return 1;
    }

    yyparse();
    fclose(file);

//Reporte
    //1
    imprimirVariablesDeclaradas(listaVariablesDeclaradas);
    liberarVariablesDeclaradas(listaVariablesDeclaradas);
    printf("\n");

    //2
    imprimirFunciones(listaFunciones);
    liberarFunciones(listaFunciones);
    printf("\n");

    //3
    imprimirSentencias(listaSentencias);
    liberarSentencias(listaSentencias);
    printf("\n");

    //4
    imprimirEstructurasNoReconocidas(listaEstructurasNoReconocidas);
    liberarEstructurasNoReconocidas(listaEstructurasNoReconocidas);
    printf("\n");

    //5
    imprimirCadenasNoReconocidas(listaCadenasNoReconocidas);
    liberarCadenasNoReconocidas(listaCadenasNoReconocidas);
    printf("\n");

    return 0;
}

	/* Definición de la funcion yyerror para reportar errores, necesaria para que la funcion yyparse del analizador sintáctico pueda invocarla para reportar un error */
/*void yyerror(const char* literalCadena)
{
        fprintf(stderr, "Bison: %d:%d: %s\n", yylloc.first_line, yylloc.first_column, literalCadena);
}

/* Fin de la sección de epílogo (código de usuario) */