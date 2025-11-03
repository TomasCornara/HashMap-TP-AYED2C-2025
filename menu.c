#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "menu.h"


char* formatearRegistro(char* buffer, char* palabra, unsigned apariciones, unsigned hsh)
{
    sprintf(buffer,"   Pal: [%s] -> Apariciones: %d -> Hash: %d\n", palabra, apariciones,hsh);
    return buffer;
}

void esperar_ms(int ms)
{
    clock_t start = clock();
    clock_t end = start + ms * CLOCKS_PER_SEC / 1000;
    while(clock() < end);
}


void printAnimacion(char* texto)
{
    char *temp = texto;

    while (*temp)
    {
        printf("%c", *temp);
        fflush(stdout);

        if (*temp == '\n')
        {
            esperar_ms(ESPERA_SALTO_LINEA);
        }
        else
        {
            esperar_ms(ESPERA_LETRA);
        }
        temp++;
    }
}

void mostrarMenu()
{
    system("cls");
    printf("\n");
    printf("  --------------------------------------------\n");
    printf("  |           MENU PRINCIPAL                 |\n");
    printf("  --------------------------------------------\n");
    printf("  |                                          |\n");
    printf("  |  1. Mostrar estadisticas generales       |\n");
    printf("  |  2. Mostrar listado de apariciones       |\n");
    printf("  |  3. Buscar palabra especifica            |\n");
    printf("  |  0. Salir                                |\n");
    printf("  |                                          |\n");
    printf("  -------------------------------------------\n");
    printf("\n  Seleccione una opcion: ");
}

int leerOpcion()
{
    int opcion;
    char buffer[10];

    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        if (sscanf(buffer, "%d", &opcion) == 1)
        {
            return opcion;
        }
    }
    return -1; // Opcion inválida
}
