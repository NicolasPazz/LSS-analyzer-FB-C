%{

#include <stdio.h>
#include "funciones.h"
#include <stddef.h>

NodoIdentificador* listaIdentificadores = NULL;
NodoLiteralCadena* listaLiteralesCadena = NULL;
NodoPalabraReservada* listaPalabrasReservadasPorTipoDeDato = NULL;
NodoPalabraReservada* listaPalabrasReservadasPorEstructuraDeControl = NULL;
NodoPalabraReservada* listaOtrasPalabrasReservadas = NULL;
NodoConstanteDecimal* listaConstantesEnterasDecimales = NULL;
NodoConstanteDecimal* listaConstantesEnterasHexadecimales = NULL;
NodoConstanteDecimal* listaConstantesEnterasOctales = NULL;
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


%%

    /* Palabras reservadas */
    /*Tipos de datos*/
"char"|"double"|"enum"|"float"|"int"|"long"|"short"|"struct"|"union"|"void"                      {agregarPalabraReservada(listaPalabrasReservadasPorTipoDeDato, linea, columna, yytext); INCREMENTAR_COLUMNA}
     /*Estructuras de control*/
"break"|"case"|"continue"|"default"|"do"|"else"|"for"|"goto"|"if"|"return"|"switch"|"while"      {agregarPalabraReservada(listaPalabrasReservadasPorEstructuraDeControl, linea, columna, yytext); INCREMENTAR_COLUMNA}
    /*Otros*/
"auto"|"const"|"extern"|"register"|"signed"|"sizeof"|"static"|"typedef"|"unsigned"|"volatile"    {agregarPalabraReservada(listaOtrasPalabrasReservadas, linea, columna, yytext); INCREMENTAR_COLUMNA}

    /* Constantes numéricas con sufijos */
[0-9]+[uU]?[lL]{0,2}                                                    {agregarConstanteEntera(listaConstantesEnterasDecimales, yytext); INCREMENTAR_COLUMNA}
0[xX][0-9a-fA-F]+[uU]?[lL]{0,2}                                         {agregarConstanteEntera(listaConstantesEnterasHexadecimales, yytext); INCREMENTAR_COLUMNA}
0[0-7]*[uU]?[lL]{0,2}                                                   {agregarConstanteEntera(listaConstantesEnterasOctales, yytext); INCREMENTAR_COLUMNA}
[0-9]+\.[0-9]*([eE][+-]?[0-9]+)?[fFlL]?                                 {agregarConstanteReal(listaConstantesReales, yytext); INCREMENTAR_COLUMNA}
[0-9]+([eE][+-]?[0-9]+)[fFlL]?                                          {agregarConstanteReal(listaConstantesReales, yytext); INCREMENTAR_COLUMNA}
\n                                                                      {INCREMENTAR_LINEA}      
\'([^\\\']|\\[abfnrtv\\\'\"\?]|\\[0-7]{1,3}|\\x[0-9a-fA-F]+)\'          {agregarConstanteCaracter(listaConstantesCaracter, yytext); INCREMENTAR_COLUMNA}

    /* Literales cadena */
\"([^\\\"]|\\[abfnrtv\\\'\"\?]|\\[0-7]{1,3}|\\x[0-9a-fA-F]+)*\"         {agregarLiteralCadena(listaLiteralesCadena, yytext); INCREMENTAR_COLUMNA}

    /* Identificadores */
[a-zA-Z_][a-zA-Z0-9_]*                                                  {agregarIdentificador(listaIdentificadores, yytext); INCREMENTAR_COLUMNA}

    /* Operadores y caracteres de puntuación */
[+\-*/%=<>&|!^~?:;,.\[\](){}]                                           {agregarOperadorYCaracteresDePuntuacion(listaOperadoresYCaracteresDePuntuacion, yytext); INCREMENTAR_COLUMNA}

    /* Cualquier otro carácter no reconocido */
.                                                                       {agregarCadenaNoReconocida(listaCadenasNoReconocidas, yytext); INCREMENTAR_COLUMNA}

%%


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s archivo.i\n", argv[0]);
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
    
    //2
    imprimirLiteralesCadena(listaLiteralesCadena);

    //3
    imprimirPalabrasReservadas(listaPalabrasReservadasPorTipoDeDato, listaPalabrasReservadasPorEstructuraDeControl, listaOtrasPalabrasReservadas);

    //4
    imprimirConstantesEnterasDecimales(listaConstantesEnterasDecimales);
    imprimirConstantesEnterasHexadecimales(listaConstantesEnterasHexadecimales);
    imprimirConstantesEnterasOctales(listaConstantesEnterasOctales);  
    imprimirConstantesReales(listaConstantesReales);
    imprimirConstantesCaracter(listaConstantesCaracter);

    //5
    imprimirOperadoresYCaracteresDePuntuacion(listaOperadoresYCaracteresDePuntuacion);

    //6
    imprimirCadenasNoReconocidas(listaCadenasNoReconocidas);

    //Funcion para liberar todos los nodos

    return 0;
}

/* Fin de la sección de código de usuario */