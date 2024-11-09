%{
#include "general.h"

extern FILE *yyin;

extern int yylex(void);

void yyerror(const char *s);

// Declaramos listas e inicializamos en null
NodoSimbolo* tablaSimbolos = NULL;
NodoSimbolo* listaVariablesDeclaradas = NULL;
NodoSimbolo* listaFunciones = NULL;
NodoErroresSemanticos* listaErroresSemanticos = NULL;
NodoErrorSintactico* listaErrorSintactico = NULL;
NodoErrorSintactico* listaSecuenciasLeidas = NULL;
NodoCadenaNoReconocida* listaCadenasNoReconocidas  = NULL;
Parametro* listaDeParametros = NULL;
NodoSimbolo* nodoGenericoFuncion = NULL;
// Variables globales definidas
//char *tipoReturnEsperado = NULL;
//TipoRetorno* tipoReturnEncontrado =NULL;
EspecificadorTipos especificadorVacio = {.esTipoDato = VACIO_TIPODATO, .esAlmacenamiento = VACIO_ESPALMAC, .esCalificador = VACIO_CALIFICADORTIPO};

EspecificadorTipos tipoRetorno;

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
    EspecificadorTipos especificadorTipos;
    tipoDato esTipoDato;
    especificadorAlmacenamiento esAlmacenamiento;
    calificadorTipo esCalificador;
}

%token <cadena> IDENTIFICADOR LITERAL_CADENA
%token OP_ASIGNACION OP_RELACIONAL OP_INCREMENTO_DECREMENTO OP_MULTIPLICATIVO OP_ADITIVO OP_IGUALDAD NO_RECONOCIDO OP_AND OP_OR BREAK CASE CONTINUE DEFAULT DO ELSE FOR IF RETURN SWITCH WHILE GOTO VOID CHAR DOUBLE ENUM FLOAT INT LONG SHORT STRUCT UNION AUTO CONST EXTERN REGISTER SIGNED SIZEOF STATIC TYPEDEF UNSIGNED VOLATILE
%token <entero> CONSTANTE_ENTERA CONSTANTE_CARACTER
%token <real> CONSTANTE_REAL

%type <cadena> lista_declaradores_funcion lista_declaradores_variable lista_declaradores_variable_prototipo   lista_argumentos_prototipo argumento_prototipo lista_declaradores_variable_for declarador_variable_for  declarador_variable_prototipo inicializacion_variable_prototipo inicializacion_variable_for declarador_variable inicializacion_variable lista_argumentos_invocacion  declarador_funcion  definicion_funcion definiciones_externas declaracion sentencia_de_salto break continue return sentencia_for sentencia_do_while sentencia_switch sentencia_etiquetada cases default case sentencia_if_else sentencia_compuesta sentencia_de_expresion expresion_op sentencias_compuestas_sin_llaves sentencias expresion expresion_primaria expresion_postfija expresion_unaria expresion_multiplicativa expresion_aditiva expresion_relacional expresion_de_igualdad expresion_and expresion_or expresion_de_asignacion 
%type <especificadorTipos> sufijo especificador_declaracion
%type <esTipoDato> tipo_de_dato
%type <esCalificador> calificador_tipo
%type <esAlmacenamiento> especificador_almacenamiento

%left OP_AND OP_OR
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
    | CONSTANTE_CARACTER                        { DBG_PRINT("expresion_primaria - CONSTANTE_CARACTER: %d\n", $1); }
    | LITERAL_CADENA                            { DBG_PRINT("expresion_primaria - LITERAL_CADENA: %s\n", $1); }
    | '(' expresion ')'                         { DBG_PRINT("expresion_primaria - (EXP)\n");}
    ;
expresion_postfija:
      IDENTIFICADOR '(' lista_argumentos_invocacion ')'     { DBG_PRINT("expresion_postfija - INVOCACION FUNCION: (argumentos)\n"); /*validarInvocacionAFuncion(&listaErroresSemanticos, $1, $3, @1.last_line, @1.first_column);*/ }
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

tipo_de_dato
    : CHAR              { $$ = CHAR_TIPODATO; }
    | VOID              { $$ = VOID_TIPODATO; }
    | DOUBLE            { $$ = DOUBLE_TIPODATO; }
    | FLOAT             { $$ = FLOAT_TIPODATO; }
    
    | INT               { $$ = INT_TIPODATO; }
    | UNSIGNED INT      { $$ = UNSIGNED_INT_TIPODATO; }

    | LONG              { $$ = LONG_TIPODATO; }
    | UNSIGNED LONG     { $$ = UNSIGNED_LONG_TIPODATO; }

    | SHORT             { $$ = SHORT_TIPODATO; }
    | UNSIGNED SHORT    { $$ = UNSIGNED_SHORT_TIPODATO; }
    ;
especificador_almacenamiento
    : AUTO               { $$ = AUTO_ESPALMAC; }
    | EXTERN             { $$ = EXTERN_ESPALMAC; }
    | REGISTER           { $$ = REGISTER_ESPALMAC; }
    | STATIC             { $$ = STATIC_ESPALMAC; }
    | TYPEDEF            { $$ = TYPEDEF_ESPALMAC; }
    ;
calificador_tipo
    : CONST             { $$ = CONST_CALIFICADORTIPO; }
    | VOLATILE          { $$ = VOLATILE_CALIFICADORTIPO; }
    ;

sufijo
    : tipo_de_dato                  { $$ = (struct EspecificadorTipos){.esTipoDato = $1, .esAlmacenamiento = VACIO_ESPALMAC, .esCalificador = VACIO_CALIFICADORTIPO}; DBG_PRINT("Tipo de dato: %s\n", especificadorTiposString[$$.esTipoDato]);}
    | especificador_almacenamiento  { $$ = (struct EspecificadorTipos){.esTipoDato = VACIO_TIPODATO, .esAlmacenamiento = $1, .esCalificador = VACIO_CALIFICADORTIPO}; DBG_PRINT("Almacenamiento: %s\n", especificadorAlmacenamientoString[$$.esAlmacenamiento]);}
    | calificador_tipo              { $$ = (struct EspecificadorTipos){.esTipoDato = VACIO_TIPODATO, .esAlmacenamiento = VACIO_ESPALMAC, .esCalificador = $1}; DBG_PRINT("Calificador: %s\n", calificadorTipoString[$$.esCalificador]);}
    ;

especificador_declaracion
    : sufijo                            { $$ = $1; DBG_PRINT("ESPECIFICADOR DECLARACION 1: Tipo de dato: %s, Almacenamiento: %s, Calificador: %s\n", especificadorTiposString[$$.esTipoDato], especificadorAlmacenamientoString[$$.esAlmacenamiento], calificadorTipoString[$$.esCalificador]);}
    | especificador_declaracion sufijo  { $$ = (combinarEspecificadorTipos($1, $2)); DBG_PRINT("ESPECIFICADOR DECLARACION 2: Tipo de dato: %s, Almacenamiento: %s, Calificador: %s\n", especificadorTiposString[$$.esTipoDato], especificadorAlmacenamientoString[$$.esAlmacenamiento], calificadorTipoString[$$.esCalificador]); }
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
    | sentencia                     { DBG_PRINT("sentencia_compuesta_sin_llaves\n"); }
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
      IF '(' expresion ')' sentencia_compuesta                                      
    | IF '(' expresion ')' sentencia_compuesta ELSE sentencia_compuesta             
    | IF '(' expresion ')' sentencia_compuesta_sin_llaves                          
    | IF '(' expresion ')' sentencia_compuesta_sin_llaves ELSE sentencia_compuesta  
    ;
sentencia_switch:
    SWITCH '(' expresion ')' '{' sentencia_etiquetada '}'    
sentencia_etiquetada:
    cases default
    ;
case:
      CASE expresion ':' sentencias_compuestas_sin_llaves   
    | CASE expresion ':' sentencias_compuestas_sin_llaves 
      BREAK                                                 
    ;
default
    : /*VACIO*/
    | DEFAULT ':' sentencias
    | DEFAULT ':' sentencias 
      BREAK
    ;
cases
    : /*VACIO*/
    | case
    | cases case
    ;

sentencia_do_while:
      WHILE '(' expresion ')' sentencia_compuesta
    | DO sentencia_compuesta WHILE '(' expresion ')' ';'
    ;
sentencia_for:
    FOR '(' primera_parte_for ';' expresion_op ';' expresion_op ')' 
    sentencia_compuesta
    ;
expresion_op
    : /*VACIO*/
    | expresion     { DBG_PRINT("expresion_op\n");}
    ;
primera_parte_for
    : /*VACIO*/
    | especificador_declaracion lista_declaradores_variable_for { DBG_PRINT("primera_parte_for\n");}
    | IDENTIFICADOR                                       { DBG_PRINT("primera_parte_for\n");}
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
    BREAK ';'       { /*agregarSentencia(&listaSentencias, "break", @1.first_line, @1.first_column);*/ }
    ;
return:
      RETURN expresion ';'     { /*registrarReturn($2, @1.first_line, @1.last_column);*/ }
    | RETURN ';'               { /*registrarReturn(NULL, @1.first_line,@1.last_column);*/ }
    ;

declaracion
    : especificador_declaracion lista_declaradores_variable ';'             { /*agregarVariableDeclarada(&listaVariablesDeclaradas, &tablaSimbolos, &listaErroresSemanticos, $3, tipoRetorno, @1.first_line, @1.first_column, $1);*/ }
    | especificador_declaracion lista_declaradores_funcion ';'              { agregarFuncion(&listaFunciones, &tablaSimbolos, $1, &nodoGenericoFuncion, @1.first_line, DECLARACION_FUNCION, @1.first_column); /*DBG_PRINT("declaracion de funcion 3 %s %s\n", $1, $2); */};
    ;

lista_declaradores_variable:
      declarador_variable                                  { DBG_PRINT("lista_declaradores_variable\n"); }
    | lista_declaradores_variable ',' declarador_variable  { DBG_PRINT("lista_declaradores_variable\n"); }
    ;
declarador_variable:
    IDENTIFICADOR inicializacion_variable   { /*agregarVariableDeclarada(&listaVariablesDeclaradas, &tablaSimbolos, &listaErroresSemanticos, $1, yyval.especificadorTipos,  @1.first_line, @1.first_column, yyval.sufijo); */DBG_PRINT("declarador_variable \n"); }
    ;
inicializacion_variable
    : /*VACIO*/
    | OP_ASIGNACION OP_ADITIVO expresion    { DBG_PRINT("inicializacion de variable \n"); }
    | OP_ASIGNACION expresion               { DBG_PRINT("inicializacion de variable \n"); }
    ;

lista_declaradores_funcion:
      declarador_funcion                                   { DBG_PRINT("declarador_funcion %s\n", $$); }
    | lista_declaradores_funcion ',' declarador_funcion    { DBG_PRINT("lista_declaradores_funcion\n"); }
    ;
declarador_funcion:
    IDENTIFICADOR '(' lista_argumentos_prototipo ')'      { llenarNodoGenericoFuncion(&nodoGenericoFuncion, $1, &listaDeParametros); DBG_PRINT("declarador_funcion %s\n", $1);}
    ;
lista_argumentos_prototipo:
      argumento_prototipo                                   { DBG_PRINT("argumento_prototipo\n"); }
    | lista_argumentos_prototipo ',' argumento_prototipo    { DBG_PRINT("argumento_prototipo\n"); }
    ;
       
argumento_prototipo
    : /*VACIO*/                                     { agregarParametro(&listaDeParametros, especificadorVacio, NULL); DBG_PRINT("argumento_prototipo_final\n"); }
    | IDENTIFICADOR                                 { agregarParametro(&listaDeParametros, especificadorVacio, $1); DBG_PRINT("argumento_prototipo_final\n"); }
    | especificador_declaracion                     { agregarParametro(&listaDeParametros, $1, NULL); DBG_PRINT("argumento_prototipo_final\n"); }
    | especificador_declaracion IDENTIFICADOR       { agregarParametro(&listaDeParametros, $1, $2); DBG_PRINT("argumento_prototipo_final\n"); }
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

definiciones_externas:
      declaracion           { DBG_PRINT("definiciones_externas: declaracion \n"); }
    | definicion_funcion   
    ;

definicion_funcion: 
      especificador_declaracion declarador_funcion sentencia_compuesta   {/*inicializarTipoRetorno($1) ; validarTipoReturn(&listaErroresSemanticos); agregarFuncion(&listaFunciones, &tablaSimbolos, $1, &nodoGenericoFuncion, @1.first_line, DEFINICION_FUNCION, @1.first_column);*/ DBG_PRINT("definiciones_externas: definicion de funcion\n");}
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
    //imprimirVariablesDeclaradas(listaVariablesDeclaradas);
    //liberarVariablesDeclaradas(listaVariablesDeclaradas); 
    printf("\n");

    //2
    //imprimirFunciones(listaFunciones);
    //liberarFunciones(listaFunciones);
    printf("\n");

   //3
    //imprimirErrorSemantico(listaErroresSemanticos);
    printf("\n");

    //4
    //imprimirErrorSintactico(listaErrorSintactico);
    //liberarErrorSintactico(listaErrorSintactico);
    printf("\n");

    //5 
    //imprimirCadenasNoReconocidas(listaCadenasNoReconocidas);
    //liberarCadenasNoReconocidas(listaCadenasNoReconocidas);
    printf("\n");
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "ERROR en linea %d columna %d: %s\n", yylloc.last_line, yylloc.last_column, s);
}