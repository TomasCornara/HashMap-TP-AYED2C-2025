#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "parseArchivos.h"

FILE* abrirArchivoTextoL(char* nombre_arch)
{
    char* aux;
    char buffer[MAX_NOMBRE];

    //Comprobar si el nombre esta bien formateado
    if((aux = strrchr(nombre_arch,'.')))
    {
        if(strcmp(aux,".txt") == 0)
        {
            sprintf(buffer,"%s",nombre_arch);
        }
        else
        {
            return NULL; //Tipo de archivo incorrecto
        }
    }
    else     //Si no pasa el formato, se lo agrego
    {
        sprintf(buffer,"%s%s",nombre_arch,".txt");
    }

    return fopen(buffer,"rt");
}

char* sigPalArch(FILE* arch, char* buffer, size_t buffer_size) {
    int c;
    size_t i = 0;

    if (buffer_size < 1) {
        return NULL;
    }

    c = fgetc(arch);
    if (c == EOF) {
        return NULL;
    }

    if (isspace(c)) {
        if (buffer_size > 1) {
            buffer[0] = c;
            buffer[1] = '\0';
            return buffer;
        }
        ungetc(c, arch);
        return NULL;
    }

    if (ispunct(c)) {
        if (buffer_size > 1) {
            buffer[0] = c;
            buffer[1] = '\0';
            return buffer;
        }
        ungetc(c, arch);
        return NULL;
    }

    buffer[i++] = c;
    int fin_palabra = 0;
    while (!fin_palabra && i < buffer_size - 1) {
        c = fgetc(arch);
        if (c == EOF || isspace(c) || ispunct(c)) {
            if (c != EOF) {
                ungetc(c, arch);
            }
            fin_palabra = 1;
        } else {
            buffer[i++] = c;
        }
    }

    buffer[i] = '\0';
    return buffer;
}

int es_signo_puntuacion(const char* cadena)
{
    char c;

    if(!cadena || cadena[0] == '\0')
    {
        return 1;
    }

    for(int i = 0; cadena[i] != '\0'; i++)
    {
        c = cadena[i];
        if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
        {
            return 0;
        }
    }
    return 1;
}
