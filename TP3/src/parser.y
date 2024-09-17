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
FILE *output_file;
%}
/* Fin de la sección de prólogo (declaraciones y definiciones de C y directivas del preprocesador) */

/* Inicio de la sección de declaraciones de Bison */
//%define parse.error verbose
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

//%type 

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
    ENTERO
    ;
    


expresionentreparentesis:
    ABROPARENTESIS expresion CIERROPARENTESIS {printf("expresionentreparentesis\n");}
    ;

sentencia:
    | sentenciadeexpresion
    | sentenciacompuesta

    | sentenciaif
    | sentenciaifelse
    | sentenciaswitch

    | sentenciawhile
    | sentenciadowhile
    | sentenciafor

    | continue
    | break
    | return
    ;

sentenciadeexpresion:
    expresion PUNTOYCOMA {printf("sentenciadeexpresion\n");}
    ;

sentenciacompuesta:
    ABROLLAVE sentencias CIERROLLAVE {printf("sentenciacompuesta\n");}
    ;

sentencias:
    sentencia
    | sentencias sentencia
    ;

sentenciaif:
    IF expresionentreparentesis sentenciacompuesta {printf("sentenciaif\n");}
    ;

sentenciaifelse:
    IF expresionentreparentesis sentenciacompuesta
    ELSE sentenciacompuesta {printf("sentenciaifelse\n");}
    ;

sentenciaswitch:
    SWITCH expresionentreparentesis ABROLLAVE cases CIERROLLAVE {printf("sentenciaswitch\n");}
    | SWITCH expresionentreparentesis ABROLLAVE cases default CIERROLLAVE {printf("sentenciaifelse\n");}
    //fijarme si el default puede ir en otra parte que no sea el final 
    ;

default:
    DEFAULT {printf("default\n");}
    ;

sentenciawhile:
    WHILE expresionentreparentesis sentenciacompuesta {printf("sentenciawhile\n");}
    ;

sentenciadowhile:
    DO sentenciacompuesta
    WHILE expresionentreparentesis PUNTOYCOMA {printf("sentenciadowhile\n");}
    ;

sentenciafor:
    FOR expresionentreparentesis sentenciacompuesta {printf("sentenciafor\n");}
    //modificar, no puede ser cualquier expresión entre paréntesis
    ;

case:
    CASE expresion DOSPUNTOS sentencias {printf("case\n");}
    ;

cases:
    case
    | cases case
    ;

continue:
    CONTINUE {printf("continue\n");}
    ;

break:
    BREAK {printf("break\n");}
    ;

return:
    RETURN expresion PUNTOYCOMA {printf("return\n");}
    ;

%%
/* Fin de la sección de reglas gramaticales */

/* Inicio de la sección de epílogo (código de usuario) */

int main(int argc, char **argv) {
    // Abrir archivo de salida
    output_file = fopen("output.txt", "w");
    if (!output_file) {
        perror("No se pudo abrir output.txt");
        return 1;
    }

    yyparse();

    fclose(output_file);
    return 0;
}

//Reporte
    //1
    /*imprimirVariablesDeclaradas(listaVariablesDeclaradas);
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
*/

	/* Definición de la funcion yyerror para reportar errores, necesaria para que la funcion yyparse del analizador sintáctico pueda invocarla para reportar un error */
/*void yyerror(const char* literalCadena)
{
        fprintf(stderr, "Bison: %d:%d: %s\n", yylloc.first_line, yylloc.first_column, literalCadena);
}

/* Fin de la sección de epílogo (código de usuario) */