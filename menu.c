#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "menu.h"


char* formatearRegistro(char* buffer, char* palabra, unsigned apariciones, unsigned hsh){
    sprintf(buffer,"   Pal: [ %s ]\n\tApariciones: %d \n\tHash: %d\n \n\n", palabra, apariciones,hsh);
    return buffer;
}

void esperar_ms(int ms) {
    clock_t start = clock();
    clock_t end = start + ms * CLOCKS_PER_SEC / 1000;
    while(clock() < end);
}


void printAnimacion(char* texto){
    char *temp = texto;

    while (*temp) {
        printf("%c", *temp);
        fflush(stdout);

        if (*temp == '\n') {
            esperar_ms(ESPERA_SALTO_LINEA);
        } else {
            esperar_ms(ESPERA_LETRA);
        }
        temp++;
    }
}
