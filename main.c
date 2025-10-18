#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_map.h"
#include "listaDinamica.h"
#include "parseArchivos.h"
#include "globales.h"
#include "menu.h"





int main()

{
    char buffer[MAX_BUFFER];
    //char aux[MAX_BUFFER];
    FILE* mi_archivo;
    mi_archivo = abrirArchivoTextoL("test.txt");
    if(!mi_archivo) return 1;

    printAnimacion(HEADER);
    getc(stdin);
    for(int i = 0; i < 10; i++){
        crearRegistro(buffer,"hola",i);
        printAnimacion(buffer);
    }
    printAnimacion(FOOTER);

    return 0;
}


