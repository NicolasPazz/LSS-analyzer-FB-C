
/* Inicio de la sección de definiciones. Aquí: */
    /* 1. Cualquier texto sangrado o encerrado entre '%{' y '%}' (estos últimos dos sin sangrar en líneas ocupadas únicamente por ellos) se copia íntegramente al archivo de salida del analizador léxico (scanner) generado (*.lex.yy.c) */
        /* De esas formas podemos poner declaraciones y definiciones de C y directivas del preprocesador */
    /* 2. Los comentarios (sólo hay de múltiples líneas) sin sangría también se copian tal cual al archivo de salida del analizador léxico (scanner) generado (*.lex.yy.c) */
%{

#include <stdio.h>
#include "funciones.h"

%}
    /* La siguiente línea define explícitamente a yytext como un puntero a un caracter ( char* yytext; ). Ésta es la opción por defecto si no se pone nada. */
%pointer
    /* Con la siguiente opción la función de análisis int yylex(void); se comporta como si en la sección de usuario existiera la función int yywrap(void) return 1; la cual devuelve verdadero (no-cero), haciendo que una vez que la función de análisis int yylex(void) reciba una indicación de EOF desde YY_INPUT, asuma que no hay nada más para analizar de yyin y finalice (haciendo un return 0;) */
%option noyywrap

    /* Condiciones de arranque inclusivas */
/* %s */
    /* Condiciones de arranque exclusivas */
/* %x */

    /* Declaraciones con cada nombre y su definición; se utilizan para simplificar las reglas */
/* digito [0-9] */

/* Fin de la sección de definiciones */

/* Inicio de la sección de reglas. Aquí: */
    /* 1. Cualquier texto sangrado o encerrado entre '%{' y '%}' (estos últimos dos sin sangrar en líneas ocupadas únicamente por ellos) se copia íntegramente al archivo de salida del analizador léxico (scanner) generado (*.lex.yy.c) */
        /* Esto podría utilizarse para declarar variables que son locales a la rutina de análisis léxico y (después de las declaraciones) al código que debe ejecutarse siempre que se entra a la rutina de análisis léxico */
    /* 2. NO pueden hacerse comentarios sin sangría, ya que estos no se copian literalmente al archivo de salida del analizador léxico (scanner) generado (*.lex.yy.c) */
%%

/* Palabras reservadas */
"auto"|"const"|"extern"|"register"|"signed"|"sizeof"|"static"|"typedef"|"unsigned"|"volatile"    { guardar_palabra_reservada(yytext); }
"char"|"double"|"enum"|"float"|"int"|"long"|"short"|"struct"|"union"|"void"                      { guardar_palabra_reservada(yytext); }
"break"|"case"|"continue"|"default"|"do"|"else"|"for"|"goto"|"if"|"return"|"switch"|"while"      { guardar_palabra_reservada(yytext); }

/* Constantes numéricas con sufijos */
[0-9]+[uU]?[lL]{0,2}                                                               { guardar_constante(yytext, "decimal"); }
0[xX][0-9a-fA-F]+[uU]?[lL]{0,2}                                                    { guardar_constante(yytext, "hexadecimal"); }
0[0-7]*[uU]?[lL]{0,2}                                                              { guardar_constante(yytext, "octal"); }
[0-9]+\.[0-9]*([eE][+-]?[0-9]+)?[fFlL]?                                            { guardar_constante(yytext, "real"); }
[0-9]+([eE][+-]?[0-9]+)[fFlL]?                                                     { guardar_constante(yytext, "real"); }
\'([^\\\']|\\[abfnrtv\\\'\"\?]|\\[0-7]{1,3}|\\x[0-9a-fA-F]+)\'                      { guardar_constante(yytext, "caracter"); }

/* Literales cadena */
\"([^\\\"]|\\[abfnrtv\\\'\"\?]|\\[0-7]{1,3}|\\x[0-9a-fA-F]+)*\"                   { guardar_literal_cadena(yytext); }

/* Identificadores */
[a-zA-Z_][a-zA-Z0-9_]*                                                            { agregarIdentificador(yytext); }

/* Operadores y caracteres de puntuación */
[+\-*/%=<>&|!^~?:;,.\[\](){}]                                                     { guardar_operador(yytext); }

/* Cualquier otro carácter no reconocido */
.                                                                                 { guardar_no_reconocida(yytext); }


    /* Reglas por defecto de Flex: */

    /* [1-9][0-9]* { printf("\nEncontre una constante entera decimal: %s\n", yytext); }
0[0-7]* { printf("\nEncontre una constante entera octal: %s\n", yytext); }
0[xX][0-9a-fA-F]+ { printf("\nEncontre una constante entera hexadecimal: %s\n", yytext); }
[_a-zA-Z]([_a-zA-Z]|[0-9])* { printf("\nEncontre un identificador: %s\n", yytext); }
*/
        /* <*>.|\n { ECHO; } */
            /* (ECHO; escribe yytext en la salida del escáner: FILE* yyout (variable global a la cual por defecto se le asigna stdout)) */
        /* <<EOF>> { yyterminate(); } */
            /* (yyterminate(); hace que la función yylex finalice retornando un 0, indicando un fin-de-entrada (EOF)) */

%%
/* Fin de la sección de reglas */ 

/* Inicio de la sección de código de usuario, la cual: */
	/* 1. Se utiliza para rutinas de complemento que llaman al analizador léxico o son llamadas por este; la presencia de esta sección es opcional */
    /* 2. Toda esta sección simplemente se copia íntegramente al archivo de salida del analizador léxico (scanner) generado (*.lex.yy.c) */


/* Fin de la sección de código de usuario */

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

    return 0;
}