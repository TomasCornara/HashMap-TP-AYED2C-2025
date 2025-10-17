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

    while(i < buffer_size - 1 && (c = fgetc(arch)) != EOF){
        if(isspace(c)){
            buffer[i] = '\0';
            return buffer;
        }
        buffer[i++] = (char)c;
    }

    if(i > 0){
        buffer[i] = '\0';
        return buffer;
    }
    return NULL;
}

char* quitarPuntuacion(char* buffer, char* buffer_signos){
    int i = 0, j = 0, k = 0;

    while(buffer[i]){
        if(ispunct(buffer[i])){
            buffer_signos[k] = buffer[i];
            k++;
        } else {
            buffer[j] = buffer[i];
            j++;
        }
        i++;
    }

    buffer[j] = '\0';
    buffer_signos[k] = '\0';

    return (k > 0) ? buffer_signos : NULL;
}
