typedef struct NodoIdentificador {
    char *identificador;
    int contador;
    struct NodoIdentificador *siguiente;
} NodoIdentificador;

NodoIdentificador* crearNodoIdentificador(const char *identificador);

void agregarIdentificador(NodoIdentificador **lista, const char *identificador);

//--

typedef struct NodoLiteralCadena {
    char *literalCadena;
    int longitud;
    struct NodoLiteralCadena *siguiente;
} NodoLiteralCadena;

NodoLiteralCadena* crearNodoLiteralCadena(const char *literalCadena);

void agregarLiteralCadena(NodoLiteralCadena **lista, const char *literalCadena);

//--

typedef struct NodoPalabraReservada {
    char *palabraReservada;
    int linea;
    int columna;
    struct NodoPalabraReservada *siguiente;
} NodoPalabraReservada;

NodoPalabraReservada* crearNodoPalabraReservada(const char *palabraReservada, int linea, int columna);

void agregarPalabraReservada(NodoPalabraReservada **lista, int linea, int columna, const char *palabraReservada);

//--

typedef struct NodoConstanteEntera {
    char *constanteEntera;
    struct NodoConstanteEntera *siguiente;
} NodoConstanteEntera;

NodoConstanteEntera* crearNodoConstanteEntera(const char *constanteEntera);

void agregarConstanteEntera(NodoConstanteEntera **lista, const char *constanteEntera);

void imprimirConstantesEnterasDecimales(NodoConstanteEntera *lista);

void imprimirConstantesEnterasHexadecimales(NodoConstanteEntera *lista);

void imprimirConstantesEnterasOctales(NodoConstanteEntera *lista);

//--

