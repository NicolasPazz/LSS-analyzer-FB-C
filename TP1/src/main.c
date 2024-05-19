#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CANT_ESTADOS 7
#define CANT_SIMBOLOS 5

// Definición de los estados del AFD
typedef enum {
    Q0,
    Q1,
    Q2,
    Q3,
    Q4,
    Q5,
    Q6
} t_estado;

typedef enum {
   CERO,
   UNO_SIETE,
   OCHO_NUEVE,
   A_F,
   EQUIS   
} t_columna;


#define ESTADO_INICIAL Q0
#define ESTADO_FINALD Q1 //solo un 0 DECIMAL
#define ESTADO_FINALO1 Q2 //solo un 0 OCTAL
#define ESTADO_FINAL02 Q5 //octal 0...
#define ESTADO_FINALH Q4 //hexadecimal 0Xx 
#define CENTINELA ','

//Q6 es el estado final de error
int tabla_transiciones[CANT_ESTADOS][CANT_SIMBOLOS] = {
    {Q2, Q1, Q1, Q6, Q6},
    {Q1, Q1, Q1, Q6, Q6},
    {Q3, Q3, Q6, Q6, Q4},
    {Q3, Q3, Q6, Q6, Q6},
    {Q5, Q5, Q5, Q5, Q6},
    {Q5, Q5, Q5, Q5, Q6},
    {Q6, Q6, Q6, Q6, Q6}
};

int cambiar_estado(int estado_actual, int tabla_transicion[CANT_ESTADOS][CANT_SIMBOLOS], int c){
    if ((c>= 'a' && c<= 'f') || (c>= 'A' && c<= 'F'))
        return tabla_transicion[estado_actual][A_F];
    if (c == '0')
        return tabla_transicion[estado_actual][CERO];
    if (c == 'x' || c == 'X')
        return tabla_transicion[estado_actual][EQUIS];
    if (c>= 1 && c<= 7)
        return tabla_transicion[estado_actual][UNO_SIETE];
    if (c>= 8 && c<= 9)
        return tabla_transicion[estado_actual][OCHO_NUEVE];
    
    return tabla_transicion[estado_actual][OTROS];
}

void lexer(FILE* input, FILE* output) {
    int c; //getchar devuelve un entero
    int estado = ESTADO_INICIAL;
    while((c = getchar(input)) != EOF){
        if(c != CENTINELA){
            fputc(c, output);
            estado = tabla_transiciones[estado][char_to_enum(c)];
        }
        else{
            fputs("    ", output);
            if(estado == ESTADO_FINAL){
                fputs("Aceptada\n", output);
            }
            else{
                 fputs("No Aceptada\n", output);
            }
            estado = ESTADO_INICIAL;
        }
    }
    fputs("    ", output);
    if(estado == ESTADO_FINAL){
                fputs("Acepatda", output);
            }
            else{
                 fputs("No Aceptada", output);
            }
}
