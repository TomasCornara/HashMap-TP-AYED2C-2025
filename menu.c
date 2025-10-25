#include <stdio.h>
#include <time.h>
#include <windows.h>
#include "menu.h"


char* crearRegistro(char* buffer, char* palabra, unsigned apariciones){
    sprintf(buffer,"    Pal: %-20s  Apariciones: %-15u\n", palabra, apariciones);
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
            esperar_ms(2);
        } else {
            esperar_ms(1);
        }
        temp++;
    }
}
