typedef struct NodoIdentificador {
    char *identificador;
    int contador;
    struct NodoIdentificador *siguiente;
} NodoIdentificador;

// Función para crear un nuevo NodoIdentificador
NodoIdentificador* crearNodoIdentificador(const char *identificador);

// Función para agregar una identificador a la lista
void agregarIdentificador(NodoIdentificador **lista, const char *identificador);

// Función para imprimir la lista
void imprimirListaIdentificador(NodoIdentificador *lista);

// Función para liberar la memoria de la lista
void liberarListaIdentificador(NodoIdentificador *lista);

--

typedef struct NodoLiteralCadena {
    char *literalCadena;
    int longitud;
    struct NodoLiteralCadena *siguiente;
} NodoLiteralCadena;

NodoLiteralCadena* crearNodoLiteralCadena(const char *literalCadena);

void agregarLiteralCadena(NodoLiteralCadena **lista, const char *literalCadena);

void imprimirListaLiteralCadena(NodoLiteralCadena *lista);

void liberarListaLiteralCadena(NodoLiteralCadena *lista);