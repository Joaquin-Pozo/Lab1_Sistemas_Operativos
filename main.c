#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Funcion que verifica si un caracter es un numero
int isDigit(char c) {
    if (c >= '0' && c <= '9') {
        return 1;
    }
    return 0;
}

// Función que lee un archivo y transforma los números de char a int
int *leerArchivo(char nombreArchivo[], int *cantidad) {
    FILE *archivo = fopen(nombreArchivo, "r");

    // Comprueba que el archivo exista en el mismo path
    if (archivo == NULL) {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        *cantidad = 0;
        return NULL;
    }

    // Crea un arreglo dinámico para almacenar el contenido del archivo
    int *arrayNumeros = malloc(1000 * sizeof(int));
    if (!arrayNumeros) {
        printf("Error asignando memoria. El archivo es muy largo\n");
        *cantidad = 0;
        fclose(archivo);
        return NULL;
    }

    int i = 0;
    int c;
    // lee el archivo hasta que no queden datos
    while ((c = fgetc(archivo)) != EOF) {
        if (isDigit((char)c)) {
            if (i < 1000) {
                // convierte el numero de char a int y lo almacena en el arreglo de enteros
                arrayNumeros[i++] = c - '0'; 
            }
        }
    }

    fclose(archivo);

    *cantidad = i;
    return arrayNumeros;
}

void conteoDigitos(int arrayNumeros[], int cantidadNumeros) {
    printf("Conteo de dígitos:\n");
    int sumaDigitos = 0;
    
    for (int i = 0; i < 5; i++) {
        int fd[2]; // arreglo de 2 elementos, [0] para lectura y [1] para escritura
        pid_t pid_hijo;
        char buffer[100];
        char mensaje_hijo[100];
        int digitos = 0;

        pipe(fd);
        pid_hijo = fork();

        for (int j = 0; j < cantidadNumeros; j++) {
            if (arrayNumeros[j] == i*2 || arrayNumeros[j] == i*2 + 1) {
                digitos++;
            }
        }
        // almacena el mensaje en buffer
        snprintf(mensaje_hijo, sizeof(mensaje_hijo), "%d-%d: %d", i*2, i*2 + 1, digitos);
        sumaDigitos = sumaDigitos + digitos;

        if (pid_hijo == 0) {
            //printf("Soy el hijo: %d, y mi padre es: %d\n", getpid(), getppid());
            close(fd[0]); // cierra el descriptor de lectura
            write(fd[1], mensaje_hijo, strlen(mensaje_hijo) + 1); // envia el mensaje al padre
            close(fd[1]); // cierra el descriptor de escritura
            exit(EXIT_SUCCESS);
        }
        //printf("Soy el proceso padre: %d, mi hijo es: %d\n", getpid(), pid_hijo);
        close(fd[1]); // para recibir el mensaje, cierra el descriptor de escritura
        read(fd[0], buffer, sizeof(buffer));
        printf("%s\n", buffer);

        close(fd[0]);
    }
    printf("Total Dígitos: %d\n", sumaDigitos);
    
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

    int cantidadNumeros = 0;
    int *arrayNumeros = leerArchivo(str, &cantidadNumeros);

    if (arrayNumeros == NULL || cantidadNumeros == 0) {
        return 0;
    }
    conteoDigitos(arrayNumeros, cantidadNumeros);

    free(arrayNumeros);

    return 0;
}