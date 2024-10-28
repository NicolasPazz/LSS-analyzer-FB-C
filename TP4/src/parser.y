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

// Declaramos listas e inicializamos en null
NodoSimbolo* tablaSimbolos = NULL;
NodoVariableDeclarada* listaVariablesDeclaradas = NULL;
NodoFuncion* listaFunciones = NULL;
NodoErroresSemanticos* listaErroresSemanticos = NULL;
NodoErrorSintactico* listaErrorSintactico = NULL;
NodoErrorSintactico* listaSecuenciasLeidas = NULL;
NodoCadenaNoReconocida* listaCadenasNoReconocidas  = NULL;

char* listaParametros = NULL;
char* parametro = NULL;

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
    char* tipoDeDato;
    char* sufijo;  
}

%token <cadena> IDENTIFICADOR SUFIJO TIPODEDATO LITERAL_CADENA OP_ASIGNACION OP_RELACIONAL OP_INCREMENTO_DECREMENTO OP_MULTIPLICATIVO OP_ADITIVO OP_IGUALDAD NO_RECONOCIDO OP_AND OP_OR BREAK CASE CONTINUE DEFAULT DO ELSE FOR IF RETURN SWITCH WHILE GOTO VOID CONSTANTE_CARACTER
%token <entero> CONSTANTE_ENTERA
%token <real> CONSTANTE_REAL


%type <cadena> lista_declaradores_funcion  sufijo lista_declaradores_variable lista_declaradores_variable_prototipo   lista_argumentos_prototipo argumento_prototipo lista_declaradores_variable_for declarador_variable_for  declarador_variable_prototipo inicializacion_variable_prototipo inicializacion_variable_for declarador_variable inicializacion_variable lista_argumentos_invocacion  declarador_funcion  definicion_funcion definiciones_externas declaracion sentencia_de_salto break continue return sentencia_for sentencia_do_while sentencia_switch sentencia_etiquetada cases default case sentencia_if_else sentencia_compuesta sentencia_de_expresion expresion_op sentencias_compuestas_sin_llaves sentencias expresion expresion_primaria expresion_postfija expresion_unaria expresion_multiplicativa expresion_aditiva expresion_relacional expresion_de_igualdad expresion_and expresion_or expresion_de_asignacion

%left OP_AND OP_OR
%left TIPODEDATO
%left '+' '-'
%left '*' '/'
%left '^'
%left '(' ')'

%start input

%%
input
    : /*VACIO*/ 
    | input line
    ;
line: 
      sentencia 
    | declaracion
    | definiciones_externas 
    | error ';'                    { agregarErrorSintactico(&listaErrorSintactico, &listaSecuenciasLeidas); yyclearin; yyerrok; DBG_PRINT("error sintactico\n"); }
    ;

expresion:
      expresion_primaria                         { DBG_PRINT("expresion - EXPRESION_PRIMARIA\n"); }
    | expresion_postfija                         { DBG_PRINT("expresion - EXPRESION_POSTFIJA\n"); }
    | expresion_unaria                           { DBG_PRINT("expresion - EXPRESION_UNARIA\n"); }
    | expresion_multiplicativa                   { DBG_PRINT("expresion - EXPRESION_MULTIPLICATIVA\n"); }
    | expresion_aditiva                          { DBG_PRINT("expresion - EXPRESION_ADITIVA\n"); }
    | expresion_relacional                       { DBG_PRINT("expresion - EXPRESION_RELACIONAL\n"); }
    | expresion_de_igualdad                      { DBG_PRINT("expresion - EXPRESION_DE_IGUALDAD\n"); }
    | expresion_and                              { DBG_PRINT("expresion - EXPRESION_AND\n"); }
    | expresion_or                               { DBG_PRINT("expresion - EXPRESION_OR\n"); }
    | expresion_de_asignacion                    { DBG_PRINT("expresion - EXPRESION_DE_ASIGNACION\n"); }
    ;
expresion_primaria: 
      IDENTIFICADOR                             { DBG_PRINT("expresion_primaria - IDENTIFICADOR: %s\n", $1); }
    | CONSTANTE_ENTERA                          { DBG_PRINT("expresion_primaria - CONSTANTE_ENTERA: %d\n", $1); }
    | CONSTANTE_REAL                            { DBG_PRINT("expresion_primaria - CONSTANTE_REAL: %f\n", $1); }
    | CONSTANTE_CARACTER                        { DBG_PRINT("expresion_primaria - CONSTANTE_CARACTER: %s\n", $1); }
    | LITERAL_CADENA                            { DBG_PRINT("expresion_primaria - LITERAL_CADENA: %s\n", $1); }
    | '(' expresion ')'                         { DBG_PRINT("expresion_primaria - (EXP)\n");}
    ;
expresion_postfija:
      IDENTIFICADOR '(' lista_argumentos_invocacion ')'     { DBG_PRINT("expresion_postfija - INVOCACION FUNCION: (argumentos)\n"); validarInvocacionAFuncion(&listaErroresSemanticos, $1, $3, yylloc.last_line, @1.first_column)}
    | IDENTIFICADOR OP_INCREMENTO_DECREMENTO                { DBG_PRINT("expresion_postfija - INCREMENTO/DECREMENTO: \n"); }
    ;
expresion_unaria:
      OP_INCREMENTO_DECREMENTO IDENTIFICADOR      { DBG_PRINT("expresion_unaria - INCREMENTO/DECREMENTO:\n"); }
    ;

expresion_multiplicativa:
      expresion OP_MULTIPLICATIVO expresion       { DBG_PRINT("expresion_multiplicativa: EXP1  EXP2\n"); }
    ;
expresion_aditiva:
      expresion OP_ADITIVO expresion              { DBG_PRINT("expresion_aditiva: EXP1 +/- EXP2\n"); } 
    ;
expresion_relacional:
      expresion OP_RELACIONAL expresion
    ;
expresion_de_igualdad:
      expresion OP_IGUALDAD expresion             { DBG_PRINT("expresion_de_igualdad: EXP1 ==/!= EXP2\n"); }
    ;
expresion_and:
      expresion_op OP_AND expresion_op            { DBG_PRINT("expresion_and\n"); }
    ;
expresion_or:
      expresion OP_OR expresion                   { DBG_PRINT("expresion_or\n"); } 
    ;
expresion_de_asignacion:
      expresion OP_ASIGNACION expresion
    ;
lista_argumentos_invocacion
    : /*VACIO*/
    | expresion                                   { DBG_PRINT("ARGUMENTO\n"); } 
    | lista_argumentos_invocacion ',' expresion   { DBG_PRINT("ARGUMENTO\n"); } 
    ;

sentencia:
      sentencia_de_expresion 
    | sentencia_compuesta 
    | sentencia_if_else 
    | sentencia_switch 
    | sentencia_do_while 
    | sentencia_for 
    | sentencia_de_salto
    //| sentencia_etiquetada: solo puede aparecer dentro de una sentencia_switch
    //| continue: solo puede aparecer dentro de una sentencia de iteracion
    //| break: solo puede aparecer dentro de una sentencia_switch
    ;
sentencia_de_expresion:
    expresion_op ';'    {DBG_PRINT("sentencia_de_expresion\n");}
    ;
sentencia_compuesta:
    '{' declaraciones sentencias '}'    {DBG_PRINT("sentencia_compuesta\n");}
    ;
sentencia_compuesta_sin_llaves:
      declaracion
    | sentencia                     {DBG_PRINT("sentencia_compuesta_sin_llaves\n");}
    | error ';'                     { agregarErrorSintactico(&listaErrorSintactico, &listaSecuenciasLeidas); yyclearin; yyerrok; DBG_PRINT("error sintactico\n"); }
    ;
sentencias_compuestas_sin_llaves
    : /*VACIO*/
    | sentencia_compuesta_sin_llaves                                    { DBG_PRINT("sentencias_compuestas_sin_llaves\n");}
    | sentencias_compuestas_sin_llaves sentencia_compuesta_sin_llaves   { DBG_PRINT("sentencias_compuestas_sin_llaves\n");}
    ;
sentencias
    : /*VACIO*/
    | sentencia             { DBG_PRINT("sentencias\n");}
    | sentencias sentencia  { DBG_PRINT("sentencias\n");}
    ;
declaraciones
    : /*VACIO*/
    | declaracion
    | declaraciones declaracion
    ;
sentencia_if_else:
      IF '(' expresion ')' sentencia_compuesta                                      { /*agregarSentencia(&listaSentencias, "if", @1.first_line, @1.first_column) ; DBG_PRINT("sentencia_if\n");*/}
    | IF '(' expresion ')' sentencia_compuesta ELSE sentencia_compuesta             { /*agregarSentencia(&listaSentencias, "if/else", @1.first_line, @1.first_column) ; DBG_PRINT("sentencia_if_else\n");*/}
    | IF '(' expresion ')' sentencia_compuesta_sin_llaves                          { /*agregarSentencia(&listaSentencias, "if", @1.first_line, @1.first_column) ; DBG_PRINT("sentencia_if\n");*/}
    | IF '(' expresion ')' sentencia_compuesta_sin_llaves ELSE sentencia_compuesta  { /*agregarSentencia(&listaSentencias, "if/else", @1.first_line, @1.first_column) ; DBG_PRINT("sentencia_if_else\n");*/}
    ;
sentencia_switch:
    SWITCH '(' expresion ')' '{' sentencia_etiquetada '}'    { /*agregarSentencia(&listaSentencias, "switch", @1.first_line, @1.first_column) ; DBG_PRINT("sentencia_switch\n");*/}
sentencia_etiquetada:
    cases default
    ;
case:
      CASE expresion ':' sentencias_compuestas_sin_llaves   { /*agregarSentencia(&listaSentencias, "case", @1.first_line, @1.first_column);*/}
    | CASE expresion ':' sentencias_compuestas_sin_llaves 
      BREAK                                                 { /*agregarSentencia(&listaSentencias, "case/break", @1.first_line, @1.first_column);*/}
    ;
default
    : /*VACIO*/
    | DEFAULT ':' sentencias    { /*agregarSentencia(&listaSentencias, "default", @1.first_line, @1.first_column); */}
    | DEFAULT ':' sentencias 
      BREAK                     { /*agregarSentencia(&listaSentencias, "default/break", @1.first_line, @1.first_column);*/}
    ;
cases
    : /*VACIO*/
    | case
    | cases case
    ;

sentencia_do_while:
      WHILE '(' expresion ')' sentencia_compuesta           { /*agregarSentencia(&listaSentencias, "while", @1.first_line, @1.first_column) ; DBG_PRINT("sentencia_while\n");*/}
    | DO sentencia_compuesta WHILE '(' expresion ')' ';'    { /*agregarSentencia(&listaSentencias, "do/while", @1.first_line, @1.first_column) ; DBG_PRINT("sentencia_do_while\n");*/}
    ;
sentencia_for:
    FOR '(' primera_parte_for ';' expresion_op ';' expresion_op ')' 
    sentencia_compuesta     { /*agregarSentencia(&listaSentencias, "for", @1.first_line, @1.first_column) ; DBG_PRINT("sentencia_for\n");*/}
    ;
expresion_op
    : /*VACIO*/
    | expresion     { DBG_PRINT("expresion_op\n");}
    ;
primera_parte_for
    : /*VACIO*/
    | sufijo TIPODEDATO lista_declaradores_variable_for { DBG_PRINT("primera_parte_for\n");}
    | IDENTIFICADOR                                     { DBG_PRINT("primera_parte_for\n");}
    ;
lista_declaradores_variable_for:
      declarador_variable_for                                       { DBG_PRINT("lista_declaradores_variable\n"); }
    | lista_declaradores_variable_for ',' declarador_variable_for   { DBG_PRINT("lista_declaradores_variable\n"); }
    ;
declarador_variable_for:
    IDENTIFICADOR inicializacion_variable
    ;
inicializacion_variable_for
    : /*VACIO*/
    | OP_ASIGNACION expresion   { DBG_PRINT("inicializacion de variable \n"); }
    ;
sentencia_de_salto:
      continue
    | break
    | return
    ;
continue:
    CONTINUE ';'    { /*agregarSentencia(&listaSentencias, "continue", @1.first_line, @1.first_column);*/ }
    ;
break:
    BREAK ';'   { /*agregarSentencia(&listaSentencias, "break", @1.first_line, @1.first_column);*/ }
    ;
return:
      RETURN expresion_op ';'  { /*agregarSentencia(&listaSentencias, "return", @1.first_line, @1.first_column);*/ }
    | RETURN ';'               { /*agregarSentencia(&listaSentencias, "return", @1.first_line, @1.first_column);*/ }
    ;


declaracion:
      sufijo TIPODEDATO lista_declaradores_variable ';'   { agregarVariableDeclarada(&listaVariablesDeclaradas, &tablaSimbolos, &listaErroresSemanticos, $3, $2, yylloc.last_line, @1.first_column, $1); }
    | TIPODEDATO lista_declaradores_variable ';'          { agregarVariableDeclarada(&listaVariablesDeclaradas, &tablaSimbolos, &listaErroresSemanticos, $2, $1, yylloc.last_line, @1.first_column, NULL); }
    | sufijo TIPODEDATO lista_declaradores_funcion ';'    { agregarFuncion(&listaFunciones, &tablaSimbolos, $2, $3, yylloc.last_line, "declaracion", @1.first_column); DBG_PRINT("declaracion de funcion 1 %s %s %s\n", $1, $2, $3);}
    | sufijo VOID lista_declaradores_funcion ';'          { agregarFuncion(&listaFunciones, &tablaSimbolos, $2, $3, yylloc.last_line, "declaracion", @1.first_column); DBG_PRINT("declaracion de funcion 2 %s %s %s\n", $1, $2, $3);}
    | TIPODEDATO lista_declaradores_funcion ';'           { agregarFuncion(&listaFunciones, &tablaSimbolos, $1, $2, yylloc.last_line, "declaracion", @1.first_column); DBG_PRINT("declaracion de funcion 3 %s %s\n", $1, $2); }
    | VOID lista_declaradores_funcion ';'                 { agregarFuncion(&listaFunciones, &tablaSimbolos, $1, $2, yylloc.last_line, "declaracion", @1.first_column); DBG_PRINT("declaracion de funcion 4 %s %s\n", $1, $2);}
    ;

lista_declaradores_variable:
      declarador_variable                                  { DBG_PRINT("lista_declaradores_variable\n"); }
    | lista_declaradores_variable ',' declarador_variable  { DBG_PRINT("lista_declaradores_variable\n"); }
    ;
declarador_variable:
    IDENTIFICADOR inicializacion_variable { /*agregarVariableDeclarada(&listaVariablesDeclaradas, &tablaSimbolos, &listaErroresSemanticos, $1, yyval.tipoDeDato, yylloc.last_line, @1.first_column, yyval.sufijo);*/ DBG_PRINT("declarador_variable \n"); }
    ;
inicializacion_variable
    : /*VACIO*/
    | OP_ASIGNACION OP_ADITIVO expresion   { DBG_PRINT("inicializacion de variable \n"); }
    | OP_ASIGNACION expresion   { DBG_PRINT("inicializacion de variable \n"); }
    ;

lista_declaradores_funcion:
      declarador_funcion                                   { DBG_PRINT("declarador_funcion %s\n", $$); }
    | lista_declaradores_funcion ',' declarador_funcion    { DBG_PRINT("lista_declaradores_funcion\n"); }
    ;
declarador_funcion:
    IDENTIFICADOR '(' lista_argumentos_prototipo ')'    { DBG_PRINT("declarador_funcion %s\n", $$);}
    ;
lista_argumentos_prototipo:
      argumento_prototipo                                   { DBG_PRINT("argumento_prototipo\n"); }
    | lista_argumentos_prototipo ',' argumento_prototipo    { DBG_PRINT("argumento_prototipo\n"); }
    ;

       
argumento_prototipo
    : /*VACIO*/                                     { parametro = ""; agregarParametro(&listaParametros, parametro); DBG_PRINT("argumento_prototipo_final_vacio \n"); }
    | IDENTIFICADOR                                 { parametro = copiarCadena($1); agregarParametro(&listaParametros, parametro); DBG_PRINT("argumento_prototipo_final %s\n", $1); }
    | TIPODEDATO IDENTIFICADOR                      { parametro = unirParametros($1,$2); $$ = copiarCadena(parametro); agregarParametro(&listaParametros, parametro); DBG_PRINT("argumento_prototipo_final_3 %s\n", parametro); }
    | TIPODEDATO                                    { parametro = copiarCadena($1); agregarParametro(&listaParametros, parametro); DBG_PRINT("argumento_prototipo_final %s\n", $1); }
    | sufijo TIPODEDATO                             { parametro = unirParametros($1,$2); agregarParametro(&listaParametros, parametro); DBG_PRINT("argumento_prototipo_final %s\n", parametro); }
    | sufijo TIPODEDATO IDENTIFICADOR               { parametro = unirParametros($2,$3); agregarParametro(&listaParametros, parametro); DBG_PRINT("argumento_prototipo_final %s\n", parametro); }
    | VOID                                          { parametro = "void"; agregarParametro(&listaParametros, parametro); DBG_PRINT("argumento_prototipo_final %s\n", $1); }
    ;
lista_declaradores_variable_prototipo:
      declarador_variable_prototipo                                               { DBG_PRINT("lista_declaradores_variable\n"); }
    | lista_declaradores_variable_prototipo ',' declarador_variable_prototipo     { DBG_PRINT("lista_declaradores_variable\n"); } 
    ;
declarador_variable_prototipo:
    IDENTIFICADOR inicializacion_variable
    ;
inicializacion_variable_prototipo
    : /*VACIO*/
    | OP_ASIGNACION expresion   { DBG_PRINT("inicializacion de variable \n"); }
    ;
sufijo
    : /*VACIO*/
    | SUFIJO    { DBG_PRINT("sufijo\n"); }
    ;

definiciones_externas:
      declaracion           { DBG_PRINT("definiciones_externas: declaracion \n"); }
    | definicion_funcion   
    ;

definicion_funcion: 
      TIPODEDATO declarador_funcion sentencia_compuesta   { agregarFuncion(&listaFunciones, &tablaSimbolos, $1, $2, yylloc.last_line, "definicion", @1.first_column); DBG_PRINT("definiciones_externas: definicion de funcion\n"); }
    | VOID declarador_funcion sentencia_compuesta         { agregarFuncion(&listaFunciones, &tablaSimbolos, $1, $2, yylloc.last_line, "definicion", @1.first_column); DBG_PRINT("definiciones_externas: definicion de funcion VOID\n"); }
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

    yyin = file;

    inicializarUbicacion();
    yyparse();   

    fclose(file);

//Reporte
    //1
    imprimirVariablesDeclaradas(listaVariablesDeclaradas);
    //liberarVariablesDeclaradas(listaVariablesDeclaradas); 
    printf("\n");

    //2
    imprimirFunciones(listaFunciones);
    liberarFunciones(listaFunciones);
    printf("\n");

   //3
    imprimirErrorSemantico(listaErroresSemanticos);
    printf("\n");

    //4
    imprimirErrorSintactico(listaErrorSintactico);
    liberarErrorSintactico(listaErrorSintactico);
    printf("\n");

    //5 
    imprimirCadenasNoReconocidas(listaCadenasNoReconocidas);
    liberarCadenasNoReconocidas(listaCadenasNoReconocidas);
    printf("\n");
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "ERROR en linea %d columna %d: %s\n\n", yylloc.last_line, yylloc.last_column, s);
}