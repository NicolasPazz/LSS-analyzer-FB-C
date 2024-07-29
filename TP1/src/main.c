#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CANT_ESTADOS 7
#define CANT_SIMBOLOS 6

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
   CERO = 0,
   UNO_SIETE = 1,
   OCHO_NUEVE = 2,
   A_F = 3,
   EQUIS = 4,
   OTROS = 5
} t_columna;

#define ESTADO_INICIAL Q0 //estado inicial
#define ESTADO_FINAL_DECIMAL Q1 //DECIMAL
#define ESTADO_FINAL_OCTAL_CERO Q2 //OCTAL (solo un 0 )
#define ESTADO_FINAL_OCTAL Q3 //OCTAL
#define ESTADO_TRANSITORIO Q4 //Ox,0X 
#define ESTADO_FINAL_HEXA Q5 //HEXADECIMAL 
#define ESTADO_ERROR Q6 //Caracter no reconocido 
#define CENTINELA ','

int tabla_transiciones[CANT_ESTADOS][CANT_SIMBOLOS] = {
    {Q2, Q1, Q1, Q6, Q6, Q6}, //q0-
    {Q1, Q1, Q1, Q6, Q6, Q6}, //q1+
    {Q3, Q3, Q6, Q6, Q4, Q6}, //q2+
    {Q3, Q3, Q6, Q6, Q6, Q6}, //q3+
    {Q5, Q5, Q5, Q5, Q6, Q6}, //q4
    {Q5, Q5, Q5, Q5, Q6, Q6}, //q5+
    {Q6, Q6, Q6, Q6, Q6, Q6}  //q6
};

int cambiar_estado(int estado_actual, int c){
    if (c == '0')
        return tabla_transiciones[estado_actual][CERO];

    else if (c>= '1' && c<= '7')
        return tabla_transiciones[estado_actual][UNO_SIETE];

    else if (c>= '8' && c<= '9')
        return tabla_transiciones[estado_actual][OCHO_NUEVE];

    else if ((c>= 'a' && c<= 'f') || (c>= 'A' && c<= 'F'))
        return tabla_transiciones[estado_actual][A_F];

    else if (c == 'x' || c == 'X')
        return tabla_transiciones[estado_actual][EQUIS];
    
    else return tabla_transiciones[estado_actual][OTROS];
}

void categorizar(int estado, FILE* output_file) {
    switch (estado) {
        case ESTADO_FINAL_DECIMAL:
            fputs(" es una constante entera de tipo Decimal\n", output_file);
            break;
        case ESTADO_FINAL_OCTAL_CERO:
            fputs(" es una constante entera de tipo Octal\n", output_file);
            break;
        case ESTADO_FINAL_OCTAL:
            fputs(" es una constante entera de tipo Octal\n", output_file);
            break;
        case ESTADO_FINAL_HEXA:
            fputs(" es una constante entera de tipo Hexadecimal\n", output_file);
            break;
        default:
            fputs(" --> No reconocida\n", output_file);
            break;
    }
}

void lexer(FILE* input_file, FILE* output_file) {
    int c;
    int estado = ESTADO_INICIAL;
    while((c = fgetc(input_file)) != EOF){
        if(c != CENTINELA){
            fputc(c, output_file);
            estado = cambiar_estado(estado, c);
        }
        else{
            categorizar(estado, output_file);
            estado = ESTADO_INICIAL;
        }
    }
    categorizar(estado, output_file);
}

int main() {
    // Intenta abrir el archivo para lectura
    FILE* input_file = fopen("entrada.txt", "r");
    if(input_file == NULL) {
        printf("Error al intentar abrir el archivo entrada.txt");
        return EXIT_FAILURE;
    }

    FILE* output_file = fopen("salida.txt", "w");
    if(output_file == NULL) {
        printf("Error al intentar crear el archivo salida.txt");
        fclose(input_file); // Cerrar el archivo de entrada
        return EXIT_FAILURE;
    }
    
    lexer(input_file, output_file);

    fclose(input_file);
    fclose(output_file);
    return EXIT_SUCCESS;
}