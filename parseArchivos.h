#ifndef PARSEARCHIVOS_H_INCLUDED
#define PARSEARCHIVOS_H_INCLUDED
#include <stdio.h>

#define MAX_NOMBRE 256


FILE* abrirArchivoTextoL(char* nombre_arch);
char* sigPalArch(FILE* arch, char* buffer, size_t buffer_size);
int es_signo_puntuacion(const char* cadena);

#endif // PARSEARCHIVOS_H_INCLUDED
