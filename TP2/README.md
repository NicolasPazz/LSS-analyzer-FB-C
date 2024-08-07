# TP2
## FLEX para reconocimiento de categorías léxicas de C

### Comentarios a considerar para la corrección del trabajo
*Supuestos, decisiones y observaciones en relación con el trabajo práctico realizado*

------------------------------------------------------------------------------------------------------
Implementar en lenguaje C un programa que permita reconocer desde un archivo .i (archivo .c preprocesado) de entrada todas las categorías léxicas de ANSI C (C89/C90).
El mismo deberá producir como salida en pantalla (salida estándar, stdout) un reporte.
Para desarrollar dicho programa se deberá utilizar Flex para la generación del código C que implemente un analizador léxico (scanner) que cumpla con lo requerido. Para la generación del reporte es necesario utilizar memoría dinámica en la implementación del analizador léxico
 
Categorías léxicas de ANSI C a reconocer:
●	Constantes
    ○	Enteras (decimales, octales, hexadecimales): con y sin sufijo.
    ○	Reales: con y sin sufijo.
    ○	Caracter: que a su vez pueden ser de tipo:
        ■	Simple (ej. 'a' , '0')
        ■	Con secuencia de escape
            ●	Simple (ej. '\n' , '\t')
            ●	Octal (ej. '\0' , '\07', '\007') 
            ●	Hexadecimal (ej. '\xF' , '\x0F')
●	Literales cadena
●	Palabras reservadas (agrupar por tipo de dato, estructuras de control y otros: ver abajo)
    ○	Tipos de dato: char , double , enum , float , int , long , short , struct , union , void
    ○	Estructuras de control: break , case , continue , default , do, else , for , goto , if , return , switch , while
    ○	Otros: auto , const , extern , register , signed , sizeof , static , typedef , unsigned , volatile
●	Identificadores
●	Caracteres de puntuación/operadores.

El reporte deberá consistir en un conjunto de listados que deberá seguir el siguiente orden:

1.	Listado de identificadores encontrados indicando la cantidad de veces que aparece cada uno de ellos. El listado debe estar ordenado alfabéticamente.
 
2.	Listado de literales cadena encontrados indicando la longitud de los mismos y ordenados por longitud ascendente. En caso de igual longitud se desempatan por orden de aparición. 
// consideramos que guarda repetidos

--------

3.	Listado de palabras reservadas en el orden en el que han aparecido en el archivo, indicando el número de línea y columna de cada una de ellas. Se debe indicar todas las apariciones de cada palabra reservada y separar el listado por tipos de datos, estructuras de control y otros.

4.	Listado de constantes indicando según su tipo:
○	Para las constantes decimales indicar el valor de cada una y el total acumulado de sumar todas ellas (ver ejemplos para ver como se muestra).
○	Para las constantes hexadecimales indicar su valor entero decimal.
○	Para las constantes octales indicar su valor entero decimal.
○	Para las constantes reales indicar el valor de su mantisa y parte entera.
○	Para las constantes caracter, enumerarlas según orden de aparición. 
Se deben listar en el orden de aparición y cada vez que se repitan por más que sea la misma.

5.	Listado de operadores/caracteres de puntuación indicando cantidad de veces que aparecen. Listarlos ordenados según su orden de primera aparición.

6.	Listado de cadenas no reconocidas indicando el número de línea y el de columna donde se encontraron.

Cuando no hay elemento de un listado se deberá imprimir en el reporte con un “-”. Ver ejemplos de test incluídos en la carpeta correspondiente al trabajo práctico.
