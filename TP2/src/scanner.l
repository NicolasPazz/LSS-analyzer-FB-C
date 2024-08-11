%{
#include <stdio.h>
#include "funciones.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

NodoIdentificador* listaIdentificadores = NULL;
NodoLiteralCadena* listaLiteralesCadena = NULL;
NodoPalabraReservada* listaPalabrasReservadasPorTipoDeDato = NULL;
NodoPalabraReservada* listaPalabrasReservadasPorEstructuraDeControl = NULL;
NodoPalabraReservada* listaOtrasPalabrasReservadas = NULL;
NodoConstanteEntera* listaConstantesEnterasDecimales = NULL;
NodoConstanteEntera* listaConstantesEnterasHexadecimales = NULL;
NodoConstanteEntera* listaConstantesEnterasOctales = NULL;
NodoConstanteReal* listaConstantesReales = NULL;
NodoConstanteCaracter* listaConstantesCaracter = NULL;
NodoOperadorYCaracterDePuntuacion* listaOperadoresYCaracteresDePuntuacion = NULL;
NodoCadenaNoReconocida* listaCadenasNoReconocidas = NULL;

int linea = 1;
int columna = 1;
int sumaDecimales = 0;

#define INCREMENTAR_COLUMNA (columna += yyleng)
#define INCREMENTAR_LINEA (linea++, columna = 1)

%}

%pointer
%option noyywrap
%x tokenNoReconocido

%%
    /*Tipos de datos*/
char|double|enum|float|int|long|short|struct|union|void                     {agregarPalabraReservada(&listaPalabrasReservadasPorTipoDeDato, linea, columna, yytext); INCREMENTAR_COLUMNA;}
    /*Estructuras de control*/
break|case|continue|default|do|else|for|goto|if|return|switch|while         {agregarPalabraReservada(&listaPalabrasReservadasPorEstructuraDeControl, linea, columna, yytext); INCREMENTAR_COLUMNA;}
    /*Otros*/
auto|const|extern|register|signed|sizeof|static|typedef|unsigned|volatile   {agregarPalabraReservada(&listaOtrasPalabrasReservadas, linea, columna, yytext); INCREMENTAR_COLUMNA;}

0[0-7]*[uU]?[lL]{0,2}                                                       {agregarConstanteEntera(&listaConstantesEnterasOctales, yytext); INCREMENTAR_COLUMNA;}
0[xX][0-9a-fA-F]+[uU]?[lL]{0,2}                                             {agregarConstanteEntera(&listaConstantesEnterasHexadecimales, yytext); INCREMENTAR_COLUMNA;}
[1-9][0-9]*[uU]?[lL]{0,2}                                                   {agregarConstanteEntera(&listaConstantesEnterasDecimales, yytext); INCREMENTAR_COLUMNA;}

[0-9]+\.[0-9]*([eE][+-]?[0-9]+)?[fFlL]?                                     {agregarConstanteReal(&listaConstantesReales, yytext); INCREMENTAR_COLUMNA;}
[0-9]+([eE][+-]?[0-9]+)[fFlL]?                                              {agregarConstanteReal(&listaConstantesReales, yytext); INCREMENTAR_COLUMNA;}
     
\'([^\\\']|\\[abfnrtv\\\'\"\?]|\\[0-7]{1,3}|\\x[0-9a-fA-F]+)\'              {agregarConstanteCaracter(&listaConstantesCaracter, yytext); INCREMENTAR_COLUMNA;}

\"([^\\\"]|\\[abfnrtv\\\'\"\?]|\\[0-7]{1,3}|\\x[0-9a-fA-F]+)*\"             {agregarLiteralCadena(&listaLiteralesCadena, yytext); INCREMENTAR_COLUMNA;}

[a-zA-Z_][a-zA-Z0-9_]*                                                      {agregarIdentificador(&listaIdentificadores, yytext); INCREMENTAR_COLUMNA;}

[+\-*/%=<>&|!^~?:;,.\[\](){}]                                               {agregarOperadorYCaracteresDePuntuacion(&listaOperadoresYCaracteresDePuntuacion, yytext); INCREMENTAR_COLUMNA;}

[ \t]                                                                       {INCREMENTAR_COLUMNA;}

\n                                                                          {INCREMENTAR_LINEA;} 

[^ \t\n] {unput(yytext[0]); BEGIN(tokenNoReconocido);}

<tokenNoReconocido>{
    " "|\t      {  
                    yyless(0);
                    INCREMENTAR_COLUMNA;
                    BEGIN(INITIAL);
                }
    \n          {
                    yyless(0);
                    INCREMENTAR_LINEA;
                    BEGIN(INITIAL);
                }   
    [^ \t\n]+   {
                    agregarCadenaNoReconocida(&listaCadenasNoReconocidas, yytext, linea, columna);
                    INCREMENTAR_COLUMNA;
                    BEGIN(INITIAL);
                }
}

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
    yyin = file;
    yylex();      
    fclose(file);

    //Reporte
    //1
    imprimirIdentificadores(listaIdentificadores);
    liberarIdentificadores(listaIdentificadores);
    printf("\n");
    //2
    imprimirLiteralesCadena(listaLiteralesCadena);
    liberarLiteralesCadena(listaLiteralesCadena);
    printf("\n");
    //3
    imprimirPalabrasReservadas(listaPalabrasReservadasPorTipoDeDato, listaPalabrasReservadasPorEstructuraDeControl, listaOtrasPalabrasReservadas);
    liberarPalabrasReservadas(listaPalabrasReservadasPorTipoDeDato);
    liberarPalabrasReservadas(listaPalabrasReservadasPorEstructuraDeControl);
    liberarPalabrasReservadas(listaOtrasPalabrasReservadas);
    printf("\n");
    //4
    imprimirConstantesEnterasDecimales(listaConstantesEnterasDecimales);
    liberarConstantesEnteras(listaConstantesEnterasDecimales);
    printf("\n");

    imprimirConstantesEnterasHexadecimales(listaConstantesEnterasHexadecimales);
    liberarConstantesEnteras(listaConstantesEnterasHexadecimales);
    printf("\n");

    imprimirConstantesEnterasOctales(listaConstantesEnterasOctales);  
    liberarConstantesEnteras(listaConstantesEnterasOctales);
    printf("\n");

    imprimirConstantesReales(listaConstantesReales);
    liberarConstantesReales(listaConstantesReales);
    printf("\n");

    imprimirConstantesCaracteres(listaConstantesCaracter);
    liberarConstantesCaracteres(listaConstantesCaracter);
    printf("\n");
    //5
    imprimirOperadoresYCaracteresDePuntuacion(listaOperadoresYCaracteresDePuntuacion);
    liberarOperadoresYCaracteresDePuntuacion(listaOperadoresYCaracteresDePuntuacion);
    printf("\n");
    //6
    imprimirCadenasNoReconocidas(listaCadenasNoReconocidas);
    liberarCadenasNoReconocidas(listaCadenasNoReconocidas);
    return 0;
}

/* Fin de la sección de código de usuario */


