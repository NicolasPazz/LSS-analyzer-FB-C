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

typedef struct NodoConstanteReal {
    char *constanteReal;             
    double parteEntera;         
    double mantisa;         
    struct NodoConstanteReal *siguiente;
} NodoConstanteReal;

NodoConstanteReal* crearNodoConstanteReal(const char *numero);

void agregarConstanteReal(NodoConstanteReal **lista, const char *numero);

void imprimirConstantesReales(NodoConstanteReal *lista);

//--

typedef struct NodoConstanteCaracter {
    char *constanteCaracter;
    struct NodoConstanteCaracter *siguiente;
} NodoConstanteCaracter;

NodoConstanteCaracter* crearNodoConstanteCaracter(const char *constante);

void agregarConstanteCaracter(NodoConstanteCaracter **lista, const char *constante);

void imprimirConstantesCaracteres(NodoConstanteCaracter *lista);

// OPERADOR
typedef struct NodoOperadorYCaracteresDePuntuacion {
    char *simbolo;
    int contador;
    struct NodoOperador *siguiente;
} NodoOperadorYCaracteresDePuntuacion;

NodoOperadorYCaracteresDePuntuacion* crearNodoOperadorYCaracteresDePuntuacion(const char *simbolo);

void agregarOperadorYCaracteresDePuntuacion(NodoOperador **head, const char *simbolo);

// Cadena No RECONOCIDA
typedef struct NodoCadenaNoReconocida {
    char *CadenaNoReconocida;
    int linea;
    int columna;
    struct NodoCadenaNoReconocida *siguiente;
} NodoCadenaNoReconocida;

NodoCadenaNoReconocida* crearNodoCadenaNoReconocida(const char *CadenaNoReconocida, int linea, int columna);

void agregarCadenaNoReconocida(NodoCadenaNoReconocida **head, const char *CadenaNoReconocida, int linea, int columna);