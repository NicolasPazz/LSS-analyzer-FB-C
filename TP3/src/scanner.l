/* Inicio de la sección de definiciones. Aquí: */

%{

#include "general.h"

/* Es indispensable que incluyamos el archivo de cabecera con las definiciones de YACC (*.tab.h) que se genera a partir del archivo fuente de YACC (*.y) */
#include "parser.tab.h"
#include <string.h>
#include <stdlib.h>

%}

/* Declaraciones con cada nombre y su definición; se utilizan para simplificar las reglas */

/* Fin de la sección de definiciones */

/* Inicio de la sección de reglas. Aquí: */

%%

if                                                              {return IF;}
else                                                            {return ELSE;}
switch                                                          {return SWITCH;}
while                                                           {return WHILE;}
do                                                              {return DO;}
for                                                             {return FOR;}
"("                                                             {return ABROPARENTESIS;}
")"                                                             {return CIERROPARENTESIS;}
"{"                                                             {return ABROLLAVE;}
"}"                                                             {return CIERROLLAVE;}
":"                                                             {return DOSPUNTOS;}
";"                                                             {return PUNTOYCOMA;}
case                                                            {return CASE;}
continue                                                        {return CONTINUE;}
break                                                           {return BREAK;}
return                                                          {return RETURN;}
default                                                         {return DEFAULT;}

[a-zA-Z_][a-zA-Z0-9_]+                                          {yylval.cadena = strdup(yytext); return IDENTIFICADOR;}
0[0-7]*[uU]?[lL]{0,2}                                           {return ENTERO;}
0[xX][0-9a-fA-F]+[uU]?[lL]{0,2}                                 {return ENTERO;}
[1-9][0-9]*[uU]?[lL]{0,2}                                       {return ENTERO;}
\'([^\\\']|\\[abfnrtv\\\'\"\?]|\\[0-7]{1,3}|\\x[0-9a-fA-F]+)\'  {return TEXTO;}
\"([^\\\"]|\\[abfnrtv\\\'\"\?]|\\[0-7]{1,3}|\\x[0-9a-fA-F]+)*\" {return TEXTO;}
\+\+|--                                                         {return MASMASOMENOSMENOS;}
\*|/                                                            {return MULTIPLICATIVAS;}
\+|-                                                            {return ADITIVAS;}
<|>|<=|>=                                                       {return RELACIONALES;}
==|!=                                                           {return DEIGUALDAD;}
=|\+=|-=|\*=|/=                                                 {return DEASIGNACION;}
char|double|enum|float|int|long|short|struct|union|void         {return TIPODEDATO;}

%%
/* Fin de la sección de reglas */ 

/* Inicio de la sección de código de usuario, la cual: */
	/* 1. Se utiliza para rutinas de complemento que llaman al analizador léxico o son llamadas por este; la presencia de esta sección es opcional */
    /* 2. Toda esta sección simplemente se copia íntegramente al archivo de salida del analizador léxico (scanner) generado (*.lex.yy.c) */

/* Fin de la sección de código de usuario */