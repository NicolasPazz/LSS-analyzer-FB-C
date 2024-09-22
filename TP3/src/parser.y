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
%token <cadena> IDENTIFICADOR SUFIJO TIPODEDATO LITERAL_CADENA NO_RECONOCIDO OP_ASIGNACION OP_RELACIONAL OP_INCREMENTO_DECREMENTO OP_MULTIPLICATIVO OP_ADITIVO OP_IGUALDAD
%token OP_AND OP_OR  BREAK CASE CONTINUE DEFAULT DO ELSE FOR GOTO IF RETURN SWITCH WHILE 

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
    | sentencia //'\n' 
    | declaracion //'\n' //
    | //definicionesexternas //'\n'
    | error '\n' { /*agregarEstructuraNoReconocida(lista,estructura,linea)*/ yyclearin; yyerrok; printf("\n");}
    ;

expresion:
      expresionprimaria                         { printf("expresion - EXPRESIONPRIMARIA\n"); } //
    | expresionpostfija                         { printf("expresion - EXPRESIONPOSTFIJA\n"); } //
    | expresionunaria                           { printf("expresion - EXPRESIONUNARIA\n"); } //
    | expresionmultiplicativa                   { printf("expresion - EXPRESIONMULTIPLICATIVA\n"); } //
    | expresionaditiva                          { printf("expresion - EXPRESIONADITIVA\n"); } //
    | expresionrelacional                       { printf("expresion - EXPRESIONRELACIONAL\n"); } //
    | expresiondeigualdad                       { printf("expresion - EXPRESIONDEIGUALDAD\n"); } //
    | expresionand                              { printf("expresion - EXPRESIONAND\n"); } //
    | expresionor                               { printf("expresion - EXPRESIONOR\n"); } //
    | expresiondeasignacion                     { printf("expresion - EXPRESIONDEASIGNACION\n"); } //
    ;
expresionprimaria:
      IDENTIFICADOR                             { printf("expresionprimaria - IDENTIFICADOR: %s\n", $1); } //
    | CONSTANTE_ENTERA                          { printf("expresionprimaria - CONSTANTE_ENTERA: %d\n", yylval.entero); } //
    | CONSTANTE_REAL                            { printf("expresionprimaria - CONSTANTE_REAL: %f\n", yylval.real); } //
  //  | CONSTANTE_CARACTER                        { printf("expresionprimaria - CONSTANTE_CARACTER: %s\n", yylval.cadena); } //
    | LITERAL_CADENA                            { printf("expresionprimaria - LITERAL_CADENA: %s\n", yylval.cadena); } //
    | '(' expresion ')'                         { printf("expresionprimaria - (EXP)\n");} //
    ;
expresionpostfija:
      IDENTIFICADOR '(' lista_argumentos_invocacion ')'     { printf("expresionpostfija - INVOCACION FUNCION: %s(argumentos)\n", $1); } //
    | IDENTIFICADOR OP_INCREMENTO_DECREMENTO                { printf("expresionpostfija - INCREMENTO/DECREMENTO: %s %s\n", $1, $2); } //
    ;
expresionunaria:
      OP_INCREMENTO_DECREMENTO IDENTIFICADOR    { printf("expresionunaria - INCREMENTO/DECREMENTO: %s %s\n", $1, $2); } //
    ;
expresionmultiplicativa:
      expresion OP_MULTIPLICATIVO expresion     { printf("expresionmultiplicativa: EXP1 %s EXP2\n", $2); } //
    ;
expresionaditiva:
      expresion OP_ADITIVO expresion           // { printf("expresionaditiva: EXP1 +/- EXP2\n"); } //
    ;
expresionrelacional:
      expresion OP_RELACIONAL expresion         { printf("expresionrelacional: EXP1 %s EXP2\n", $2); } //
    ;
expresiondeigualdad:
      expresion OP_IGUALDAD expresion           { printf("expresiondeigualdad: EXP1 %s EXP2\n", $2); } //
    ;
expresionand:
      expresion OP_AND expresion                { printf("expresionand\n"); } //
    ;
expresionor:
      expresion OP_OR expresion                 { printf("expresionor\n"); } //
    ;
expresiondeasignacion:
      IDENTIFICADOR OP_ASIGNACION expresion     { printf("expresiondeasignacion: %s %s EXP\n", $1, $2); } //
    ;
lista_argumentos_invocacion:
    | expresion                                     { printf("ARGUMENTO\n"); } //
    | lista_argumentos_invocacion ',' expresion     { printf("ARGUMENTO\n"); } //
    ;
/*-----------------------------------------------------------------------------------------------------------*/
sentencia:
      sentenciadeexpresion //
    | sentenciacompuesta //
    | sentenciaif //
    | sentenciaifelse //
    | sentenciaswitch //
    | sentenciawhile //
    | sentenciadowhile //
    | sentenciafor
    //| sentenciaetiquetada: solo puede aparecer dentro de una sentenciaswitch
    | sentenciadesalto //
    //| continue: solo puede aparecer dentro de una sentencia de iteracion
    //| break: solo puede aparecer dentro de una sentenciaswitch
    //| declaracion
    ;
sentenciadeexpresion:
    expresionop ';' {printf("sentenciadeexpresion\n");}
    ;
sentenciacompuesta:
    '{' declaraciones sentencias '}' {printf("sentenciacompuesta\n");}
    ;
sentencias:
    | sentencia { printf("sentencias\n");}
    | sentencias sentencia { printf("sentencias\n");}
    ;
declaraciones:
    | declaracion
    | declaraciones declaracion
    ;
sentenciaif:
    IF '(' expresion ')' sentenciacompuesta { /*agregarSentencia(listaSentencias,"if",linea,columna)*/ printf("sentenciaif\n");}
    ;
sentenciaifelse:
    //revisar si cuando tengo un ifelse me lo toma como sentencia if y sentencia ifelse
    sentenciaif ELSE sentenciacompuesta { /*agregarSentencia(listaSentencias,"if/else",linea,columna)*/ printf("sentenciaifelse\n");}
    ;
sentenciaswitch:
    SWITCH '(' expresion ')' '{' sentenciaetiquetada '}'    { /*agregarSentencia(listaSentencias,"switch",linea,columna)*/ printf("sentenciaswitch\n");}
sentenciaetiquetada:
    cases default
    ;
case:
      CASE expresion ':' sentencias  { printf("sentenciaetiquetada: case\n");}
    | CASE expresion ':' sentencias 
      BREAK                          { printf("sentenciaetiquetada: case\n");}
    ;
default:
    | DEFAULT ':' sentencias    { printf("sentenciaetiquetada: default\n");}
    | DEFAULT ':' sentencias 
      BREAK                     { printf("sentenciaetiquetada: default\n");}
    ;
cases:
    | case
    | cases case
    ;
sentenciawhile:
    WHILE '(' expresion ')' sentenciacompuesta { /*agregarSentencia(listaSentencias,"while",linea,columna)*/ printf("sentenciawhile\n");}
    ;
sentenciadowhile:
    DO sentenciacompuesta
    WHILE '(' expresion ')' ';' { /*agregarSentencia(listaSentencias,"do/while",linea,columna)*/ printf("sentenciadowhile\n");}
    ;
sentenciafor:
    FOR '(' primerapartefor ';' expresionop ';' expresionop ')' 
    sentenciacompuesta { /*agregarSentencia(listaSentencias,"for",linea,columna)*/ printf("sentenciafor\n");}
    //Definir primerapartefor - es una declaracion e inicializacion de variable
    //for (inicializacion; condicion; actualizacion)
    ;
expresionop:
    | expresion { printf("expresionop\n");}
    ;
primerapartefor:
    | sufijo TIPODEDATO listadeclaradoresvariable { printf("primerapartefor\n");}
    | IDENTIFICADOR                               { printf("primerapartefor\n");}
    ;
sentenciadesalto:
      continue
    | break
    | return
    ;
continue:
    CONTINUE ';' { printf("sentenciadesalto: continue\n");}
    ;
break:
    BREAK ';' { printf("sentenciadesalto: break\n");}
    ;
return:
      RETURN expresionop ';'  { printf("sentenciadesalto: return\n");}
    | RETURN ';'            { printf("sentenciadesalto: return\n");}
    ;
/*-----------------------------------------------------------------------------------------------------------*/
declaracion:
      sufijo TIPODEDATO listadeclaradoresvariable ';'   { printf("declaracion de variable %s\n", $2); }
    | sufijo TIPODEDATO listadeclaradoresfuncion ';'    { printf("declaracion de funcion %s\n", $2); }
    ;

listadeclaradoresvariable:
    declaradorvariable                                  { printf("listadeclaradoresvariable\n"); }
    | listadeclaradoresvariable ',' declaradorvariable  { printf("listadeclaradoresvariable\n"); }
    ;
declaradorvariable:
    IDENTIFICADOR inicializacionvariable { /*verificar declaracion en ts*/; /*agregarVariableDeclarada(lista,identificador,sufijo,tipodedato,linea)*/; printf("declarador_variable %s\n", $1); }
    ;
inicializacionvariable:
    | OP_ASIGNACION expresion { printf("inicializacion de variable %s\n", $1); }
    ;

listadeclaradoresfuncion:
    declaradorfuncion                                   { printf("lista_declaradores_funcion\n"); }
    | listadeclaradoresfuncion ',' declaradorfuncion    { printf("lista_declaradores_funcion\n"); }
    ;
declaradorfuncion:
      IDENTIFICADOR '(' lista_argumentos_prototipo ')' { /*verificar declaracion en ts*/; /*agregarFuncion(lista,identificador,declaracion,listadodeparametros,tipodedato,linea)*/; printf("declarador_funcion %s\n", $1); }
    ;
lista_argumentos_prototipo:
    | argumento_prototipo                                   { printf("argumento_prototipo\n"); }
    | lista_argumentos_prototipo ',' argumento_prototipo    { printf("argumento_prototipo\n"); }
    ;
argumento_prototipo:
     declaradorvariable
    | TIPODEDATO declaradorvariable
    | TIPODEDATO
    ;

sufijo:
    | SUFIJO { printf("sufijo %s\n", $1); }
    ;

/*-----------------------------------------------------------------------------------------------------------*/

definicionesexternas:
    declaracion
    |definicion_de_funciones
;

definicion_de_funciones: 
sufijo TIPODEDATO listadeclaradoresfuncion sentenciacompuesta { printf("declaracion de funcion %s\n", $<cadena>3); }
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