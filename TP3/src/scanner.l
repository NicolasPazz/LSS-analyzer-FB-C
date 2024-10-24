%{
#include <string.h>
#include <stdlib.h>
#include "general.h"
#include "parser.tab.h"
extern char* copiarCadena(const char *str);
%}

%pointer
%option noyywrap
%x tokenNoReconocido

%%
    /*Operadores*/
"("                                                                         {yylloc.last_column += yyleng; return '(';}
")"                                                                         {yylloc.last_column += yyleng; return ')';}
"{"                                                                         {yylloc.last_column += yyleng; return '{';}
"}"                                                                         {yylloc.last_column += yyleng; return '}';}
";"                                                                         {yylloc.last_column += yyleng; return ';';}
","                                                                         {yylloc.last_column += yyleng; return ',';}
":"                                                                         {yylloc.last_column += yyleng; return ':';}
"="|"*="|"/="|"+="|"-="                                                     {yylval.cadena = copiarCadena(yytext); yylloc.last_column += yyleng; return OP_ASIGNACION;}
"<"|">"|"<="|">="                                                           {yylval.cadena = copiarCadena(yytext); yylloc.last_column += yyleng; return OP_RELACIONAL;}
"&&"                                                                        {yylloc.last_column += yyleng; return OP_AND;}
"||"                                                                        {yylloc.last_column += yyleng; return OP_OR;}
"++"|"--"                                                                   {yylval.cadena = copiarCadena(yytext); yylloc.last_column += yyleng; return OP_INCREMENTO_DECREMENTO;}
[*/]                                                                        {yylval.cadena = copiarCadena(yytext); yylloc.last_column += yyleng; return OP_MULTIPLICATIVO;}
[+-]                                                                        {yylloc.last_column += yyleng; return OP_ADITIVO;}
"=="|"!="                                                                   {yylval.cadena = copiarCadena(yytext); yylloc.last_column += yyleng; return OP_IGUALDAD;}

    /*Tipos de datos*/
char|double|enum|float|int|long|short|struct|union                          {yylval.cadena = copiarCadena(yytext); yylloc.last_column += yyleng; return TIPODEDATO;}
void                                                                        {yylval.cadena = copiarCadena(yytext); yylloc.last_column += yyleng; return VOID;}

    /*Estructuras de control*/
break                                                                       {yylloc.last_column += yyleng; return BREAK;}
case                                                                        {yylloc.last_column += yyleng; return CASE;}
continue                                                                    {yylloc.last_column += yyleng; return CONTINUE;}
default                                                                     {yylloc.last_column += yyleng; return DEFAULT;}
do                                                                          {yylloc.last_column += yyleng; return DO;}
else                                                                        {yylloc.last_column += yyleng; return ELSE;}
for                                                                         {yylloc.last_column += yyleng; return FOR;}
goto                                                                        {yylloc.last_column += yyleng; return GOTO;}
if                                                                          {yylloc.last_column += yyleng; return IF;}
return                                                                      {yylloc.last_column += yyleng; return RETURN;}
switch                                                                      {yylloc.last_column += yyleng; return SWITCH;}
while                                                                       {yylloc.last_column += yyleng; return WHILE;}

    /*Otros*/
auto|const|extern|register|signed|sizeof|static|typedef|unsigned|volatile   {yylval.cadena = copiarCadena(yytext); yylloc.last_column += yyleng; return SUFIJO;}

    /*Constantes enteras*/
0[0-7]*[uU]?[lL]{0,2}                                                       {yylval.entero = strtol(yytext, NULL, 8); yylloc.last_column += yyleng; return CONSTANTE_ENTERA;}
0[xX][0-9a-fA-F]+[uU]?[lL]{0,2}                                             {yylval.entero = strtol(yytext, NULL, 16); yylloc.last_column += yyleng; return CONSTANTE_ENTERA;}
[1-9][0-9]*[uU]?[lL]{0,2}                                                   {yylval.entero = strtol(yytext, NULL, 10); yylloc.last_column += yyleng; return CONSTANTE_ENTERA;}

    /*Constantes reales*/
[0-9]+\.[0-9]*([eE][+-]?[0-9]+)?[fFlL]?                                     {yylval.real = strtof(yytext, NULL); yylloc.last_column += yyleng; return CONSTANTE_REAL;}
[0-9]+([eE][+-]?[0-9]+)[fFlL]?                                              {yylval.real = strtof(yytext, NULL); yylloc.last_column += yyleng; return CONSTANTE_REAL;}

    /*Literal cadena*/
\"([^\\\"]|\\[abfnrtv\\\'\"\?]|\\[0-7]{1,3}|\\x[0-9a-fA-F]+)*\"             {yylval.cadena = copiarCadena(yytext); yylloc.last_column += yyleng; return LITERAL_CADENA;}

    /*Identificadores*/
[a-zA-Z_][a-zA-Z0-9_]*                                                      {yylval.cadena = copiarCadena(yytext); yylloc.last_column += yyleng; return IDENTIFICADOR;}

    /*Espacio y tabulacion*/
[ \t]                                                                       {yylloc.first_column += yyleng;}

    /*Salto de linea*/
\n                                                                          { yylloc.first_line = yylloc.last_line += yyleng; yylloc.first_column = yylloc.last_column = 1; /*return '\n'; printf("\n\n"); yylloc.last_line += 1;*/}

[^ \t\n]                                                                    {unput(yytext[0]); BEGIN(tokenNoReconocido);}

<tokenNoReconocido>{
    <<EOF>>     {
                    BEGIN(INITIAL);
                }
    " "|\t|\n   {
                    yyless(0);
                    BEGIN(INITIAL);
                }   
    [^ \t\n]+   {
                    yylval.cadena = copiarCadena(yytext);
                    agregarCadenaNoReconocida(&listaCadenasNoReconocidas, yytext, yylloc.last_line, yylloc.first_column); 
                    yylloc.last_column += yyleng;
                    BEGIN(INITIAL); 
                    return NO_RECONOCIDO;
                }
}

    /*Fin de archivo*/
<<EOF>> {return EOF;}
%%