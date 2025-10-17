#ifndef PARSEARCHIVOS_H_INCLUDED
#define PARSEARCHIVOS_H_INCLUDED
#include <stdio.h>

#define MAX_NOMBRE 256


FILE* abrirArchivoTextoL(char* nombre_arch);
int esCharPunt(char ch);
char* sigPalArch(FILE* arch, char* buffer, size_t buffer_size);
char* quitarPuntuacion(char* buffer, char* buffer_signos);

#endif // PARSEARCHIVOS_H_INCLUDED
