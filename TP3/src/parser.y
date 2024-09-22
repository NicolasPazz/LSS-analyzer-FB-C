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
NodoVariableDeclarada* listaVariablesDeclaradas = NULL;
NodoFuncion* listaFunciones = NULL;
NodoSentencia* listaSentencias = NULL;
NodoEstructuraNoReconocida* listaEstructurasNoReconocidas = NULL;
NodoCadenaNoReconocida* listaCadenasNoReconocidas  = NULL;

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
    struct yylval_struct{
        int entero;
        float real;
        char* cadena;
        char* tipo;
        char* sufijo;
    } mystruct;
}

%token <mystruct> CONSTANTE_ENTERA CONSTANTE_REAL IDENTIFICADOR SUFIJO TIPODEDATO LITERAL_CADENA OP_ASIGNACION OP_RELACIONAL OP_INCREMENTO_DECREMENTO OP_MULTIPLICATIVO OP_ADITIVO OP_IGUALDAD NO_RECONOCIDO
%token OP_AND OP_OR BREAK CASE CONTINUE DEFAULT DO ELSE FOR IF RETURN SWITCH WHILE GOTO VOID

%type <mystruct> expresion
%type <mystruct> sentencia
%type <mystruct> declaracion
%type <mystruct> definiciones_externas

%left OP_AND OP_OR
%left TIPODEDATO
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
    | expresion //'\n'  //
    | sentencia //'\n' //
    | declaracion //'\n' //
    | definiciones_externas //'\n' 
    | error '\n' { agregarEstructuraNoReconocida(lista, estructura, yylloc.first_line);/* yyclearin; yyerrok; printf("\n");*/}
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
    | LITERAL_CADENA                            { DBG_PRINT("expresion_primaria - LITERAL_CADENA: %s\n", yylval.cadena); } //
    | '(' expresion ')'                         { DBG_PRINT("expresion_primaria - (EXP)\n");} //
    ;
expresion_postfija:
      IDENTIFICADOR '(' lista_argumentos_invocacion ')'     { DBG_PRINT("expresion_postfija - INVOCACION FUNCION: (argumentos)\n"); } //
    | IDENTIFICADOR OP_INCREMENTO_DECREMENTO                { DBG_PRINT("expresion_postfija - INCREMENTO/DECREMENTO: \n"); } //
    ;
expresion_unaria:
      OP_INCREMENTO_DECREMENTO IDENTIFICADOR      { DBG_PRINT("expresion_unaria - INCREMENTO/DECREMENTO:\n"); } //
    ;
expresion_multiplicativa:
      expresion OP_MULTIPLICATIVO expresion       { DBG_PRINT("expresion_multiplicativa: EXP1  EXP2\n"); } //
    ;
expresion_aditiva:
      expresion OP_ADITIVO expresion              { DBG_PRINT("expresion_aditiva: EXP1 +/- EXP2\n"); } //
    ;
expresion_relacional:
      expresion OP_RELACIONAL expresion           {  } //
    ;
expresion_de_igualdad:
      expresion OP_IGUALDAD expresion             { DBG_PRINT("expresion_de_igualdad: EXP1 ==/!= EXP2\n"); } //
    ;
expresion_and:
      expresion OP_AND expresion                  { DBG_PRINT("expresion_and\n"); } //
    ;
expresion_or:
      expresion OP_OR expresion                   { DBG_PRINT("expresion_or\n"); } //
    ;
expresion_de_asignacion:
      IDENTIFICADOR OP_ASIGNACION expresion       { } //
    ;
lista_argumentos_invocacion:
    | expresion                                   { DBG_PRINT("ARGUMENTO\n"); } //
    | lista_argumentos_invocacion ',' expresion   { DBG_PRINT("ARGUMENTO\n"); } //
    ;
/*-----------------------------------------------------------------------------------------------------------*/
sentencia:
      sentencia_de_expresion //{yylloc.last_line = yylloc.first_line ; yylloc.first_column = yylloc.last_column};
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
sentencia_compuesta_sin_llaves:
      declaracion
    | sentencia {DBG_PRINT("sentencia_compuesta_sin_llaves\n");}
    ;
sentencias_compuestas_sin_llaves:
    | sentencia_compuesta_sin_llaves                                    { DBG_PRINT("sentencias_compuestas_sin_llaves\n");}
    | sentencias_compuestas_sin_llaves sentencia_compuesta_sin_llaves   { DBG_PRINT("sentencias_compuestas_sin_llaves\n");}
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
    IF '(' expresion ')' sentencia_compuesta { agregarSentencia(&listaSentencias, "if", yylloc.first_line, yylloc.last_column) ; DBG_PRINT("sentencia_if\n");}
    ;
sentencia_if_else:
    //revisar si cuando tengo un ifelse me lo toma como sentencia if y sentencia ifelse
    IF '(' expresion ')' sentencia_compuesta ELSE sentencia_compuesta { agregarSentencia(&listaSentencias, "if/else", yylloc.first_line, yylloc.last_column) ; DBG_PRINT("sentencia_if_else\n");}
    ;
sentencia_switch:
    SWITCH '(' expresion ')' '{' sentencia_etiquetada '}'    { agregarSentencia(&listaSentencias, "switch", yylloc.first_line, yylloc.last_column) ; DBG_PRINT("sentencia_switch\n");}
sentencia_etiquetada:
    cases default
    ;
case:
      CASE expresion ':' sentencias_compuestas_sin_llaves   { agregarSentencia(&listaSentencias, "case", yylloc.first_line, yylloc.last_column);}
    | CASE expresion ':' sentencias_compuestas_sin_llaves 
      BREAK                                                 { agregarSentencia(&listaSentencias, "case/break", yylloc.first_line, yylloc.last_column);}
    ;
default:
    | DEFAULT ':' sentencias    { agregarSentencia(&listaSentencias, "default", yylloc.first_line, yylloc.last_column); }
    | DEFAULT ':' sentencias 
      BREAK                     { agregarSentencia(&listaSentencias, "default/break", yylloc.first_line, yylloc.last_column);}
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
    WHILE '(' expresion ')' ';' { agregarSentencia(&listaSentencias, "do/while",yylloc.first_line, yylloc.last_column) ; DBG_PRINT("sentencia_do_while\n");}
    ;
sentencia_for:
    FOR '(' primera_parte_for ';' expresion_op ';' expresion_op ')' 
    sentencia_compuesta { agregarSentencia(&listaSentencias, "for",yylloc.first_line, yylloc.last_column) ; DBG_PRINT("sentencia_for\n");}
    //Definir primera_parte_for - es una declaracion e inicializacion de variable
    //for (inicializacion; condicion; actualizacion)
    ;
expresion_op:
    | expresion { DBG_PRINT("expresion_op\n");}
    ;
primera_parte_for:
    | sufijo TIPODEDATO lista_declaradores_variable_for { DBG_PRINT("primera_parte_for\n");}
    | IDENTIFICADOR                                 { DBG_PRINT("primera_parte_for\n");}
    ;
lista_declaradores_variable_for:
    declarador_variable_for                                    { DBG_PRINT("lista_declaradores_variable\n"); }
    | lista_declaradores_variable_for ',' declarador_variable_for  { DBG_PRINT("lista_declaradores_variable\n"); }
    ;
declarador_variable_for:
    IDENTIFICADOR inicializacion_variable { }
    ;
inicializacion_variable_for:
    | OP_ASIGNACION expresion { DBG_PRINT("inicializacion de variable \n"); }
    ;
sentencia_de_salto:
      continue
    | break
    | return
    ;
continue:
    CONTINUE ';' { agregarSentencia(&listaSentencias, "continue", yylloc.last_line, yylloc.last_column); }
    ;
break:
    BREAK ';' { agregarSentencia(&listaSentencias, "break", yylloc.last_line, yylloc.last_column); }
    ;
return:
      RETURN expresion_op ';'  { agregarSentencia(&listaSentencias, "return", yylloc.last_line, yylloc.last_column); }
    | RETURN ';'               { agregarSentencia(&listaSentencias, "return", yylloc.last_line, yylloc.last_column); }
    ;
/*-----------------------------------------------------------------------------------------------------------*/
declaracion:
      sufijo TIPODEDATO lista_declaradores_variable ';'   { /*agregarVariableDeclarada(&listaVariablesDeclaradas, $<mystruct>3.cadena, $<mystruct>2.tipo, yylloc.last_line, $<mystruct>1.sufijo);*/ DBG_PRINT("declaracion de variable/s %s\n", $<mystruct>3.cadena); $<mystruct>1.sufijo = NULL;}
    | TIPODEDATO lista_declaradores_variable ';'          { /*agregarVariableDeclarada(&listaVariablesDeclaradas, $<mystruct>2.cadena, $<mystruct>1.tipo, yylloc.last_line, $<mystruct>1.sufijo);*/ DBG_PRINT("declaracion de variable/s \n"); }
    
    | sufijo TIPODEDATO lista_declaradores_funcion ';'    { DBG_PRINT("declaracion de funcion/es \n")}
    | sufijo VOID lista_declaradores_funcion ';'          { DBG_PRINT("declaracion de funcion/es VOID\n"); }
    | TIPODEDATO lista_declaradores_funcion ';'           { DBG_PRINT("declaracion de funcion/es \n") }
    | VOID lista_declaradores_funcion ';'                 { DBG_PRINT("declaracion de funcion/es VOID\n"); }
    | error                                               //{ yyclearin; yyerrok; printf("\n"); }
    ;

lista_declaradores_variable:
    declarador_variable                                    { DBG_PRINT("lista_declaradores_variable\n"); }
    | lista_declaradores_variable ',' declarador_variable  { DBG_PRINT("lista_declaradores_variable\n"); }
    ;
declarador_variable:
    IDENTIFICADOR inicializacion_variable { agregarVariableDeclarada(&listaVariablesDeclaradas, $<mystruct>1.cadena, yyval.mystruct.tipo, yylloc.last_line, yyval.mystruct.sufijo); DBG_PRINT("declarador_variable \n"); yyval.mystruct.sufijo = NULL; }
    ;
inicializacion_variable:
    | OP_ASIGNACION expresion { DBG_PRINT("inicializacion de variable \n"); }
    ;

lista_declaradores_funcion:
      declarador_funcion                                   { DBG_PRINT("lista_declaradores_funcion\n"); }
    | lista_declaradores_funcion ',' declarador_funcion    { DBG_PRINT("lista_declaradores_funcion\n"); }
    ;
declarador_funcion:
    IDENTIFICADOR '(' lista_argumentos_prototipo ')'       { /*agregarFuncion(&listaFunciones, $<mystruct>1.cadena, yylloc.last_line, "declaracion", $<mystruct>2.cadena, retorna)*/ }
    ;
lista_argumentos_prototipo:
                                                            { /* agregarRetornoFuncion(&listaFunciones, $<mystruct>1.cadena, retorna); */ }
    | argumento_prototipo                                   { DBG_PRINT("argumento_prototipo\n"); }
    | lista_argumentos_prototipo ',' argumento_prototipo    { DBG_PRINT("argumento_prototipo\n"); }
    ;
argumento_prototipo:
      declarador_variable_prototipo
    | TIPODEDATO declarador_variable_prototipo
    | TIPODEDATO
    | VOID
    ;
lista_declaradores_variable_prototipo:
    declarador_variable_prototipo                                               { DBG_PRINT("lista_declaradores_variable\n"); }
    | lista_declaradores_variable_prototipo ',' declarador_variable_prototipo   { DBG_PRINT("lista_declaradores_variable\n"); }
    ;
declarador_variable_prototipo:
    IDENTIFICADOR inicializacion_variable {  }
    ;
inicializacion_variable_prototipo:
    | OP_ASIGNACION expresion { DBG_PRINT("inicializacion de variable \n"); }
    ;
sufijo:
    | SUFIJO { DBG_PRINT("sufijo\n"); }
    | /*vacio*/
    ;
/*-----------------------------------------------------------------------------------------------------------*/
definiciones_externas:
      declaracion               { DBG_PRINT("definiciones_externas: declaracion \n"); }
    | definicion_funcion   
    ;

definicion_funcion: 
      TIPODEDATO definidor_funcion sentencia_compuesta   { DBG_PRINT("definiciones_externas: definicion de funcion\n"); }
    | VOID definidor_funcion sentencia_compuesta         { DBG_PRINT("definiciones_externas: definicion de funcion VOID\n"); }
    ;
definidor_funcion:
    IDENTIFICADOR '(' lista_argumentos_definicion ')' //{ agregarFuncion(&listaFunciones,identificador,declaracion,listadodeparametros,tipodedato,linea); DBG_PRINT("definidor_funcion\n"); }
    ;
lista_argumentos_definicion:
    | argumento_definicion                                    { DBG_PRINT("argumento_definicion\n"); }
    | lista_argumentos_definicion ',' argumento_definicion    { DBG_PRINT("argumento_definicion\n"); }
    ;
argumento_definicion:
    | sufijo TIPODEDATO IDENTIFICADOR
    | TIPODEDATO IDENTIFICADOR
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
    //liberarVariablesDeclaradas(&listaVariablesDeclaradas); //PENDIENTE
    printf("\n");

    //2
    //imprimirFunciones(listaFunciones);
    //liberarFunciones(listaFunciones);
    //printf("\n");

    //3
    imprimirSentencias(listaSentencias);
    liberarSentencias(&listaSentencias);
    printf("\n");

    //4
    //imprimirEstructurasNoReconocidas(listaEstructurasNoReconocidas);
    //liberarEstructurasNoReconocidas(listaEstructurasNoReconocidas);
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