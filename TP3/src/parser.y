%{
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include "general.h"

extern FILE *yyin;

extern int yylex(void);

void yyerror(const char *s);

NodoSentencia* listaSentencias = NULL;


#define DEBUG 0

#if DEBUG
    #define DBG_PRINT(...) fprintf(stderr, __VA_ARGS__)
#else
    #define DBG_PRINT(...)
#endif
%}

%error-verbose
%locations

%union {
    int entero;
    float real;
    char* cadena;
}

%token <entero> CONSTANTE_ENTERA
%token <real> CONSTANTE_REAL
%token <cadena> IDENTIFICADOR SUFIJO TIPODEDATO LITERAL_CADENA OP_ASIGNACION OP_RELACIONAL OP_INCREMENTO_DECREMENTO OP_MULTIPLICATIVO OP_ADITIVO OP_IGUALDAD NO_RECONOCIDO
%token OP_AND OP_OR BREAK CASE CONTINUE DEFAULT DO ELSE FOR IF RETURN SWITCH WHILE GOTO VOID

//%type <cadena> expresion
%left OP_AND OP_OR
%left '+' '-'
%left '*' '/'
%left '^'
%left '(' ')'

%start input

%%
// las gramaticas que tienen // al costado es porque ya fueron probadas y funcionan
input:
    | input line
    ;
line: 
      '\n'
    | expresion //'\n'  //
    | sentencia //'\n' //
    | declaracion //'\n' //
    | definiciones_externas //'\n' 
    | NO_RECONOCIDO { DBG_PRINT("NO_RECONOCIDO\n"); } //
    //| error '\n' { agregarEstructuraNoReconocida(lista, estructura, yylloc.first_line); yyclearin; yyerrok; printf("\n");}
    ;

expresion:
      expresion_primaria                         { DBG_PRINT("expresion - EXPRESION_PRIMARIA\n"); } //
    | expresion_postfija                         { DBG_PRINT("expresion - EXPRESION_POSTFIJA\n"); } //
    | expresion_unaria                           { DBG_PRINT("expresion - EXPRESION_UNARIA\n"); } //
    | expresion_multiplicativa                   { DBG_PRINT("expresion - EXPRESION_MULTIPLICATIVA\n"); } //
    | expresion_aditiva                          { DBG_PRINT("expresion - EXPRESION_ADITIVA\n"); } //
    | expresion_relacional                       { DBG_PRINT("expresion - EXPRESION_RELACIONAL\n"); } //
    | expresion_de_igualdad                      { DBG_PRINT("expresion - EXPRESION_DE_IGUALDAD\n"); } //
    | expresion_and                              { DBG_PRINT("expresion - EXPRESION_AND\n"); } //
    | expresion_or                               { DBG_PRINT("expresion - EXPRESION_OR\n"); } //
    | expresion_de_asignacion                    { DBG_PRINT("expresion - EXPRESION_DE_ASIGNACION\n"); } //
    ;
expresion_primaria:
      IDENTIFICADOR                             { DBG_PRINT("expresion_primaria - IDENTIFICADOR: %s\n", $1); } //
    | CONSTANTE_ENTERA                          { DBG_PRINT("expresion_primaria - CONSTANTE_ENTERA: %d\n", yylval.entero); } //
    | CONSTANTE_REAL                            { DBG_PRINT("expresion_primaria - CONSTANTE_REAL: %f\n", yylval.real); } //
//  | CONSTANTE_CARACTER                        { DBG_PRINT("expresion_primaria - CONSTANTE_CARACTER: %s\n", yylval.cadena); } //
    | LITERAL_CADENA                            //{ DBG_PRINT("expresion_primaria - LITERAL_CADENA: %s\n", yylval.cadena); } //
    | '(' expresion ')'                         { DBG_PRINT("expresion_primaria - (EXP)\n");} //
    ;
expresion_postfija:
      IDENTIFICADOR '(' lista_argumentos_invocacion ')'     { DBG_PRINT("expresion_postfija - INVOCACION FUNCION: %s(argumentos)\n", $1); } //
    | IDENTIFICADOR OP_INCREMENTO_DECREMENTO                { DBG_PRINT("expresion_postfija - INCREMENTO/DECREMENTO: %s %s\n", $1, $2); } //
    ;
expresion_unaria:
      OP_INCREMENTO_DECREMENTO IDENTIFICADOR      { DBG_PRINT("expresion_unaria - INCREMENTO/DECREMENTO: %s %s\n", $1, $2); } //
    ;
expresion_multiplicativa:
      expresion OP_MULTIPLICATIVO expresion       { DBG_PRINT("expresion_multiplicativa: EXP1 %s EXP2\n", $2); } //
    ;
expresion_aditiva:
      expresion OP_ADITIVO expresion              { DBG_PRINT("expresion_aditiva: EXP1 +/- EXP2\n"); } //
    ;
expresion_relacional:
      expresion OP_RELACIONAL expresion           { DBG_PRINT("expresion_relacional: EXP1 %s EXP2\n", $2); } //
    ;
expresion_de_igualdad:
      expresion OP_IGUALDAD expresion             { DBG_PRINT("expresion_de_igualdad: EXP1 %s EXP2\n", $2); } //
    ;
expresion_and:
      expresion OP_AND expresion                  { DBG_PRINT("expresion_and\n"); } //
    ;
expresion_or:
      expresion OP_OR expresion                   { DBG_PRINT("expresion_or\n"); } //
    ;
expresion_de_asignacion:
      IDENTIFICADOR OP_ASIGNACION expresion       { DBG_PRINT("expresion_de_asignacion: %s %s EXP\n", $1, $2); } //
    ;
lista_argumentos_invocacion:
    | expresion                                   { DBG_PRINT("ARGUMENTO\n"); } //
    | lista_argumentos_invocacion ',' expresion   { DBG_PRINT("ARGUMENTO\n"); } //
    ;
/*-----------------------------------------------------------------------------------------------------------*/
sentencia:
      sentencia_de_expresion //
    | sentencia_compuesta //
    | sentencia_if //
    | sentencia_if_else //
    | sentencia_switch //
    | sentencia_while //
    | sentencia_do_while //
    | sentencia_for
    //| sentencia_etiquetada: solo puede aparecer dentro de una sentencia_switch
    | sentencia_de_salto //
    //| continue: solo puede aparecer dentro de una sentencia de iteracion
    //| break: solo puede aparecer dentro de una sentencia_switch
    //| declaracion
    ;
sentencia_de_expresion:
    expresion_op ';' {DBG_PRINT("sentencia_de_expresion\n");}
    ;
sentencia_compuesta:
    '{' declaraciones sentencias '}' {DBG_PRINT("sentencia_compuesta\n");}
    ;
sentencias:
    | sentencia             { DBG_PRINT("sentencias\n");}
    | sentencias sentencia  { DBG_PRINT("sentencias\n");}
    ;
declaraciones:
    | declaracion
    | declaraciones declaracion
    ;
sentencia_if:
    IF '(' expresion ')' sentencia_compuesta { agregarSentencia(&listaSentencias, "if", yylloc.first_line, yylloc.first_column) ; DBG_PRINT("sentencia_if\n");}
    ;
sentencia_if_else:
    //revisar si cuando tengo un ifelse me lo toma como sentencia if y sentencia ifelse
    sentencia_if ELSE sentencia_compuesta { agregarSentencia(&listaSentencias, "if/else", yylloc.last_line, yylloc.last_column) ; DBG_PRINT("sentencia_if_else\n");}
    ;
sentencia_switch:
    SWITCH '(' expresion ')' '{' sentencia_etiquetada '}'    { agregarSentencia(&listaSentencias, "switch", yylloc.last_line, yylloc.last_column) ; DBG_PRINT("sentencia_switch\n");}
sentencia_etiquetada:
    cases default
    ;
case:
      CASE expresion ':' sentencias  { DBG_PRINT("sentencia_etiquetada: case\n");}
    | CASE expresion ':' sentencias 
      BREAK                          { DBG_PRINT("sentencia_etiquetada: case\n");}
    ;
default:
    | DEFAULT ':' sentencias    { DBG_PRINT("sentencia_etiquetada: default\n");}
    | DEFAULT ':' sentencias 
      BREAK                     { DBG_PRINT("sentencia_etiquetada: default\n");}
    ;
cases:
    | case
    | cases case
    ;
sentencia_while:
    WHILE '(' expresion ')' sentencia_compuesta { agregarSentencia(&listaSentencias, "while", yylloc.last_line, yylloc.last_column) ; DBG_PRINT("sentencia_while\n");}
    ;
sentencia_do_while:
    DO sentencia_compuesta
    WHILE '(' expresion ')' ';' { agregarSentencia(&listaSentencias, "do/while", yylloc.last_line, yylloc.last_column) ; DBG_PRINT("sentencia_do_while\n");}
    ;
sentencia_for:
    FOR '(' primera_parte_for ';' expresion_op ';' expresion_op ')' 
    sentencia_compuesta { agregarSentencia(&listaSentencias, "for", yylloc.last_line, yylloc.last_column) ; DBG_PRINT("sentencia_for\n");}
    //Definir primera_parte_for - es una declaracion e inicializacion de variable
    //for (inicializacion; condicion; actualizacion)
    ;
expresion_op:
    | expresion { DBG_PRINT("expresion_op\n");}
    ;
primera_parte_for:
    | sufijo TIPODEDATO lista_declaradores_variable { DBG_PRINT("primera_parte_for\n");}
    | IDENTIFICADOR                               { DBG_PRINT("primera_parte_for\n");}
    ;
sentencia_de_salto:
      continue
    | break
    | return
    ;
continue:
    CONTINUE ';' { DBG_PRINT("sentencia_de_salto: continue\n");}
    ;
break:
    BREAK ';' { DBG_PRINT("sentencia_de_salto: break\n");}
    ;
return:
      RETURN expresion_op ';'  { DBG_PRINT("sentencia_de_salto: return\n");}
    | RETURN ';'            { DBG_PRINT("sentencia_de_salto: return\n");}
    ;
/*-----------------------------------------------------------------------------------------------------------*/
declaracion:
      sufijo TIPODEDATO lista_declaradores_variable ';'   { DBG_PRINT("declaracion de variable/s \n"); }
    | TIPODEDATO lista_declaradores_variable ';'          { DBG_PRINT("declaracion de variable/s \n"); }
    | sufijo TIPODEDATO lista_declaradores_funcion ';'    { DBG_PRINT("declaracion de funcion/es %s\n", $2); }
    | error                                               //{ yyclearin; yyerrok; printf("\n"); }
    ;

lista_declaradores_variable:
    declarador_variable                                  { DBG_PRINT("lista_declaradores_variable\n"); }
    | lista_declaradores_variable ',' declarador_variable  { DBG_PRINT("lista_declaradores_variable\n"); }
    ;
declarador_variable:
    IDENTIFICADOR inicializacion_variable { /*agregarVariableDeclarada(lista, strdup($1), sufijo, tipodedato);*/ DBG_PRINT("declarador_variable \n"); }
    ;
inicializacion_variable:
    | OP_ASIGNACION expresion { DBG_PRINT("inicializacion de variable \n"); }
    ;

lista_declaradores_funcion:
    declarador_funcion                                     { DBG_PRINT("lista_declaradores_funcion\n"); }
    | lista_declaradores_funcion ',' declarador_funcion    { DBG_PRINT("lista_declaradores_funcion\n"); }
    ;
declarador_funcion:
      IDENTIFICADOR '(' lista_argumentos_prototipo ')' { /*agregarFuncion(lista,identificador,declaracion,listadodeparametros,tipodedato,linea);*/ DBG_PRINT("declarador_funcion %s\n", $1); }
    ;
lista_argumentos_prototipo:
    | argumento_prototipo                                   { DBG_PRINT("argumento_prototipo\n"); }
    | lista_argumentos_prototipo ',' argumento_prototipo    { DBG_PRINT("argumento_prototipo\n"); }
    ;
argumento_prototipo:
     declarador_variable
    | TIPODEDATO declarador_variable
    | TIPODEDATO
    ;
sufijo:
    | SUFIJO { DBG_PRINT("sufijo %s\n", $1); }
    | /*vacio*/
    ;
/*-----------------------------------------------------------------------------------------------------------*/
definiciones_externas:
      declaracion               { DBG_PRINT("definiciones_externas: declaracion \n"); }
    | definicion_funcion   
    ;

definicion_funcion: 
    TIPODEDATO definidor_funcion sentencia_compuesta   { DBG_PRINT("definiciones_externas: declaracion de funcion %s\n", $<cadena>3); }
    ;
definidor_funcion:
    IDENTIFICADOR '(' lista_argumentos_definicion ')' { /*agregarFuncion(lista,identificador,declaracion,listadodeparametros,tipodedato,linea);*/ DBG_PRINT("definidor_funcion %s\n", $1); }
    ;
lista_argumentos_definicion:
    | argumento_definicion                                    { DBG_PRINT("argumento_definicion\n"); }
    | lista_argumentos_definicion ',' argumento_definicion    { DBG_PRINT("argumento_definicion\n"); }
    ;
argumento_definicion:
    | sufijo TIPODEDATO IDENTIFICADOR
    | VOID
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

    inicializarUbicacion();

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

void yyerror(const char *s) {
    fprintf(stderr, "ERROR en linea %d columna %d: %s\n", yylloc.last_line, yylloc.last_column, s);
}