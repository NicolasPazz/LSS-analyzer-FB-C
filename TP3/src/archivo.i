for (a;a<10;) {
    a++;
}

for (int i = 0; i < 10; i++) {
        printf("Valor de i: %d\n", i);
}

for (int i = 0, j = 10; i < j; i++) {
        printf("i = %d, j = %d\n", i, j);
}

for (;;) {
    printf("Ejemplo 1: bucle infinito\n");
    break;
}

for (int i = 0; i < 5; i++) {
    printf("Ejemplo 2: %d\n", i);
}

for (int j = 0; j < 5; j += 2) {
    printf("Ejemplo 3: %d\n", j);
}

for (int k = 1, l = 5; k < 5 && l > 0; k++) {
    printf("Ejemplo 4: %d - %d\n", k, l);
}

for (int m = 10; m > 0; m--) {
    printf("Ejemplo 5: %d\n", m);
}

for (int n = 0; n < 10; n++) {
    if (n2 != 0) {
        continue;
    }
    printf("Ejemplo 6: %d es par\n", n);
}

