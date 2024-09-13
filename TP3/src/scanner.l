/* Inicio de la sección de definiciones. Aquí: */

%{
#include <string.h>
#include <stdlib.h>
#include "general.h"

/* Es indispensable que incluyamos el archivo de cabecera con las definiciones de YACC (*.tab.h) que se genera a partir del archivo fuente de YACC (*.y) */
#include "parser.tab.h"

%}

%pointer

%option noyywrap

/* Declaraciones con cada nombre y su definición; se utilizan para simplificar las reglas */
digito [0-9]
digitoOctal [0-7]
digitoHexa [0-9a-fA-F]
/* Fin de la sección de definiciones */

/* Inicio de la sección de reglas. Aquí: */

%%

if                                                                          {yylloc.last_column += yyleng; return IF;}

else                                                                        {yylloc.last_column += yyleng; return ELSE;}

switch                                                                      {yylloc.last_column += yyleng; return SWITCH;}

while                                                                       {yylloc.last_column += yyleng; return WHILE;}

do                                                                          {yylloc.last_column += yyleng; return DO;}

for                                                                         {yylloc.last_column += yyleng; return FOR;}

"("                                                                         {yylloc.last_column += yyleng; return ABROPARENTESIS;}

")"                                                                         {yylloc.last_column += yyleng; return CIERROPARENTESIS;}

"{"                                                                         {yylloc.last_column += yyleng; return ABROLLAVE;}

"}"                                                                         {yylloc.last_column += yyleng; return CIERROLLAVE;}

":"                                                                         {yylloc.last_column += yyleng; return DOSPUNTOS;}

";"                                                                         {yylloc.last_column += yyleng; return PUNTOYCOMA;}

case                                                                        {yylloc.last_column += yyleng; return CASE;}

continue                                                                    {yylloc.last_column += yyleng; return CONTINUE;}

break                                                                       {yylloc.last_column += yyleng; return BREAK;}

return                                                                      {yylloc.last_column += yyleng; return RETURN;}

default                                                                     {yylloc.last_column += yyleng; return DEFAULT;}

{digito}+                                                                   {yylloc.last_column += yyleng; return ENTERO;} /*para pruebas*/

[a-zA-Z_][a-zA-Z0-9_]*                                                      {yylloc.last_column += yyleng; yylval.cadena = strdup

(yytext); return IDENTIFICADOR;}

\'([^\\\']|\\[abfnrtv\\\'\"\?]|\\{digitoOctal}{1,3}|\\x{digitoHexa}+)\'     {yylloc.last_column += yyleng; return TEXTO;}

\"([^\\\"]|\\[abfnrtv\\\'\"\?]|\\{digitoOctal}{1,3}|\\x{digitoHexa}+)*\"    {yylloc.last_column += yyleng; return TEXTO;}

\+\+|--                                                                     {yylloc.last_column += yyleng; return MASMASOMENOSMENOS;}

[*/]                                                                        {yylloc.last_column += yyleng; return MULTIPLICATIVAS;}

[+-]                                                                        {yylloc.last_column += yyleng; return ADITIVAS;}

[ \t\n]+                                                                    {yylloc.last_column += yyleng;}

.                                                                           {yylloc.last_column += yyleng;}

<|>|<=|>=                                                                   {yylloc.last_column += yyleng; return RELACIONALES;}

==|!=                                                                       {yylloc.last_column += yyleng; return DEIGUALDAD;}

=|\+=|-=|\*=|/=                                                             {yylloc.last_column += yyleng; return DEASIGNACION;}

char|double|enum|float|int|long|short|struct|union|void                     {yylloc.last_column += yyleng; return TIPODEDATO;}

%%
/* Fin de la sección de reglas */ 

/* Inicio de la sección de código de usuario, la cual: */
	/* 1. Se utiliza para rutinas de complemento que llaman al analizador léxico o son llamadas por este; la presencia de esta sección es opcional */
    /* 2. Toda esta sección simplemente se copia íntegramente al archivo de salida del analizador léxico (scanner) generado (*.lex.yy.c) */

/* Fin de la sección de código de usuario */