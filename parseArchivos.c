#include <string.h>
#include <ctype.h>

#include "parseArchivos.h"

FILE* abrirArchivoTextoL(char* nombre_arch) {
    char* aux;
    char buffer[MAX_NOMBRE];

    //Comprobar si el nombre esta bien formateado
    if((aux = strrchr(nombre_arch,'.'))) {
        if(strcmp(aux,".txt") == 0) {
            sprintf(buffer,"%s",nombre_arch);
        } else {
            return NULL; //Tipo de archivo incorrecto
        }
    } else { //Si no pasa el formato, se lo agrego
        sprintf(buffer,"%s%s",nombre_arch,".txt");
    }

    return fopen(buffer,"rt");
}

char* sigPalArch(FILE* arch, char* buffer, size_t buffer_size){
    int c;
    size_t i = 0;

    // Leer el siguiente caracter
    c = fgetc(arch);

    // Si llegamos al final del archivo sin encontrar nada
    if(c == EOF){
        return NULL;
    }

    // Si es un \n, saltarlo y buscar el siguiente token
    if(c == '\n'){
        return sigPalArch(arch, buffer, buffer_size);
    }

    // Si es un signo de puntuacion o espacio (pero no \n), devolverlo como una palabra de un solo caracter
    if(ispunct(c) || (isspace(c) && c != '\n')){
        if(buffer_size > 1){
            buffer[0] = (char)c;
            buffer[1] = '\0';
            return buffer;
        }
        return NULL; // Buffer muy pequeño
    }

    // Es el inicio de una palabra normal, agregar el primer caracter
    buffer[i++] = (char)c;

    // Leer el resto de la palabra (caracteres no espacios y no puntuación)
    while(i < buffer_size - 1 && (c = fgetc(arch)) != EOF){
        if(isspace(c) || ispunct(c)){
            fseek(arch, -1, SEEK_CUR);
            break;
        }
        buffer[i++] = (char)c;
    }

    buffer[i] = '\0';
    return buffer;
}
