%{

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "general.h"

extern FILE *yyin;

extern int yylex(void);

void yyerror(const char *s) {fprintf(stderr, "Error: %s\n", s);}

%}

%error-verbose
%locations

%union {
    int entero;
    float real;
    char* cadena;
}

%token <entero> CONSTANTE_ENTERA CONSTANTE_CARACTER
%token <real> CONSTANTE_REAL
%token <cadena> IDENTIFICADOR LITERAL_CADENA NO_RECONOCIDO OP_ASIGNACION OP_RELACIONAL OP_INCREMENTO_DECREMENTO OP_MULTIPLICATIVO OP_ADITIVO OP_IGUALDAD
%token OP_AND OP_OR TIPODEDATO BREAK CASE CONTINUE DEFAULT DO ELSE FOR GOTO IF RETURN SWITCH WHILE SUFIJO

%type <cadena> expresion

%left '+' '-'
%left '*' '/'
%left '^'
%left '(' ')'

%start input

%%

input:
    | input line
    ;
line: 
      '\n'
    | expresion '\n'
    | sentencia '\n'
    | error '\n' { yyclearin; yyerrok; }
    ;
expresion:
      IDENTIFICADOR                             { printf("expresionprimaria - IDENTIFICADOR: %s\n", $1); free($1); } //
    | CONSTANTE_ENTERA                          { printf("expresionprimaria - CONSTANTE_ENTERA: %d\n", $1); } //
    | CONSTANTE_REAL                            { printf("expresionprimaria - CONSTANTE_REAL: %f\n", $1); } //
    | CONSTANTE_CARACTER                        { printf("expresionprimaria - CONSTANTE_CARACTER: %s\n", $1); free($1); } //
    | LITERAL_CADENA                            { printf("expresionprimaria - LITERAL_CADENA: %s\n", $1); free($1); } //
    | '(' expresion ')'                         { printf("expresionprimaria - (EXPRESION)\n");} //
    | IDENTIFICADOR '(' lista_argumentos ')'    { printf("expresionpostfija - INVOCACION FUNCION: %s\n", $1); free($1); } //
    | IDENTIFICADOR OP_INCREMENTO_DECREMENTO    { printf("expresionpostfija - INCREMENTO/DECREMENTO: %s %s\n", $1, $2); free($1); free($2); } //
    | OP_INCREMENTO_DECREMENTO IDENTIFICADOR    { printf("expresionunaria - INCREMENTO/DECREMENTO: %s %s\n", $1, $2); free($1); free($2); } //
    | expresion OP_MULTIPLICATIVO expresion     { printf("expresionmultiplicativa: %s %s %s\n", $1, $2, $3); free($1); free($2); free($3); } //
    | expresion OP_ADITIVO expresion            { printf("expresionaditiva: %s %s %s\n", $1, $2, $3); free($1); free($2); free($3); } //
    | expresion OP_RELACIONAL expresion         { printf("expresionrelacional: %s %s %s\n", $1, $2, $3); free($1); free($2); free($3); } //
    | expresion OP_IGUALDAD expresion           { printf("expresiondeigualdad: %s %s %s\n", $1, $2, $3); free($1); free($2); free($3); } //
    | expresion OP_AND expresion                { printf("expresionand\n"); } //
    | expresion OP_OR expresion                 { printf("expresionor\n"); } //
    | IDENTIFICADOR OP_ASIGNACION expresion     { printf("expresiondeasignacion: %s %s %s\n", $1, $2, $3); free($1); free($2); free($3); } //
    ;
lista_argumentos:
    | expresion                         { printf("ARGUMENTO\n"); } //
    | lista_argumentos ',' expresion    { printf("ARGUMENTO\n"); } //
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
    | declaracion
    ;
sentenciadeexpresion:
    expresion ';' {printf("sentenciadeexpresion\n");}
    ;
sentenciacompuesta:
    '{' sentencias '}' {printf("sentenciacompuesta\n");}
    ;
sentencias:
    sentencia
    | sentencias sentencia
    ;
sentenciaif:
    IF '(' expresion ')' sentenciacompuesta {printf("sentenciaif\n");}
    ;
sentenciaifelse:
//revisar si cuando tengo un ifelse me lo toma como sentencia if y sentencia ifelse
    IF '(' expresion ')' sentenciacompuesta
    ELSE sentenciacompuesta {printf("sentenciaifelse\n");}
    ;
sentenciaswitch:
    SWITCH '(' expresion ')' '{' cases '}' {printf("sentenciaswitch\n");}
    | SWITCH '(' expresion ')' '{' cases default '}' {printf("sentenciaifelse\n");}
    //fijarme si el default puede ir en otra parte que no sea el final 
    ;
default:
    DEFAULT  ':' sentencias {printf("default\n");}
    ;
sentenciawhile:
    WHILE '(' expresion ')' sentenciacompuesta {printf("sentenciawhile\n");}
    ;
sentenciadowhile:
    DO sentenciacompuesta
    WHILE '(' expresion ')' ';' {printf("sentenciadowhile\n");}
    ;
sentenciafor:
    //FOR '(' primerapartefor ';' expresion ';' expresion ')' 
    sentenciacompuesta {printf("sentenciafor\n");}
    //Definir primerapartefor - es una declaracion e inicializacion de variable
    ;
case:
    CASE expresion ':' sentencias {printf("case\n");}
    ;

cases:
    case
    | cases case
    ;
continue:
    CONTINUE ';' {printf("continue\n");}
    ;
break:
    BREAK ';' {printf("break\n");}
    ;
return:
    //podria ir tambien '(' expresion ')'
    RETURN expresion ';' {printf("return\n");}
    ;

declaracion:
    TIPODEDATO listadeclaradores ';' { printf("declaracion\n"); }
    ;

listadeclaradores:
    //creo que falta ';'
    declarador { printf("lista_declaradores\n"); }
    | listadeclaradores ',' declarador { printf("lista_declaradores\n"); }
    ;

declarador:
    IDENTIFICADOR
    | IDENTIFICADOR OP_ASIGNACION inicializacion
    ;

inicializacion:
    CONSTANTE_ENTERA
    | CONSTANTE_REAL
    | CONSTANTE_CARACTER
    ;

%%

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

    //init_table();     

    yyin = file;

    yyparse();   

    fclose(file);

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
    return 0;
}