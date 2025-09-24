#include <stdio.h>
#include <string.h>

// Funcion que verifica si un caracter es un numero
int isDigit(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    }
    return 0;
}

// Función que lee un archivo y transforma los números de char a int
void leerArchivo(char nombreArchivo[]) {
    FILE *archivo = fopen(nombreArchivo, "r");

    // Comprueba que el archivo exista en el mismo path
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return;
    }

    int numeros[1000];
    int i = 0;

    int c;
    while ((c = fgetc(archivo)) != EOF) {
        if (isDigit((char)c)) {
            if (i < (int)(sizeof(numeros) / sizeof(numeros[0]))) {
                // convierte el numero de char a int
                numeros[i++] = c - '0'; 
            }
        }
    }

    fclose(archivo);

    // Imprime los números encontrados
    printf("Numeros: ");
    for (int j = 0; j < i; j++) {
        printf("%d ", numeros[j]);
    }
    printf("\n");

}

int main () {
    char str[100];

    printf("Ingrese el nombre del archivo: ");

    if (fgets(str, sizeof(str), stdin) == NULL) {
        printf("Error al leer el nombre del archivo\n");
        return 1;
    }

    // elimina el salto de linea que deja fgets
    str[strcspn(str, "\n")] = '\0';

    printf("El nombre del archivo es: %s\n", str);

    leerArchivo(str);

    return 0;
}