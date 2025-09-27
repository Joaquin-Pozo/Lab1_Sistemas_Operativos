#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/*
Universidad de Santiago de Chile
Laboratorio N°1: Sistemas Operativos
Estudiante: Joaquín Pozo Garrao
Rut: 20.237.059-4
Ejecutado y compilado en Ubuntu 22.04.5 LTS
*/

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
    // Almacena la suma del total de conteo de digitos
    int sumaDigitos = 0;

    // Crea los 5 procesos hijos que leen los numeros y se comunican mediante pipes con el proceso padre
    for (int i = 0; i < 5; i++) {
        // arreglo de 2 elementos, [0] para lectura y [1] para escritura 
        int fd[2];
        pipe(fd);

        // crea un hijo
        pid_t pid_hijo;
        pid_hijo = fork();

        if (pid_hijo < 0) {
            printf("Error al crear el proceso hijo");
            exit(EXIT_FAILURE);
        }
        
        if (pid_hijo == 0) {
            int digitos = 0;
            // cuenta los digitos del arreglo
            for (int j = 0; j < cantidadNumeros; j++) {
                if (arrayNumeros[j] == i*2 || arrayNumeros[j] == i*2 + 1) {
                    digitos++;
                }
            }
            // cierra el descriptor de lectura del hijo
            close(fd[0]);
            // envia los numeros contados al padre
            write(fd[1], &digitos, sizeof(int));
            // cierra el descriptor de escritura del hijo
            close(fd[1]);
            exit(EXIT_SUCCESS);
        } else {
            // espera a que el hijo termine antes de leer el pipe
            wait(NULL);
            // cierra el descriptor de escritura del padre
            close(fd[1]);
            int digitos_contados = 0;
            read(fd[0], &digitos_contados, sizeof(int));
            // cierra el descriptor de lectura del padre
            close(fd[0]);
            // muestra los digitos contados
            printf("%d-%d: %d\n", i*2, i*2 + 1, digitos_contados);
            sumaDigitos = sumaDigitos + digitos_contados;
        }
        
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
        printf("El archivo se encuentra vacío o no existe\n");
        return 0;
    }

    conteoDigitos(arrayNumeros, cantidadNumeros);

    free(arrayNumeros);

    return 0;
}