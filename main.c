#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_map.h"
#include "listaDinamica.h"
#include "parseArchivos.h"
#include "globales.h"
#include "menu.h"

///Estructura y funciones para esta implementacion en particular
typedef struct {
    char texto[MAX_BUFFER];
    unsigned apariciones;
}t_Registro;

unsigned hash_tRegistro(const void* elem);
int cmp_tRegistro(const void* elem1, const void* elem2);
void registroDuplicado(void* elemOriginal, const void* elemEntrante);
void mostrarRegistro(const void* elem);
void mostrarHashMap(t_HashMap* hm);

///Programa
int main(int argc, char *argv[])
{
    ///Bloque declarativo
    char* nombre_arch;
    FILE* arch;
    t_HashMap diccionario;
    t_Registro buffer_registro;



    ///Bloque verificacion
    if(argc != 2){
        fprintf(stderr,"Error: Cantidad de argumentos incorrecta. Uso: ./programa <nombre_archivo>\n");
        return -1;
    }


    ///Bloque inicializacion
    nombre_arch = argv[1];
    if(!(arch = abrirArchivoTextoL(nombre_arch))){
        fprintf(stderr,"Error: No se pudo abrir correctamente el archivo");
        return -2;
    }
    crear_hash_map(&diccionario);
    buffer_registro.apariciones = 0;

    ///Bloque proceso
    while(sigPalArch(arch,buffer_registro.texto,MAX_BUFFER)){
        buffer_registro.apariciones = 1;  // Inicializar apariciones en 1

        //Poner la palabra o signo de puntuacion
        poner_en_hmap(&diccionario,&buffer_registro,sizeof(t_Registro),hash_tRegistro,cmp_tRegistro,registroDuplicado);
    }

    ///Mostrar resultados
    mostrarHashMap(&diccionario);

    ///Bloque limpieza
    vaciar_hash_map(&diccionario);
    fclose(arch);


    return 0;
}

///Implementacion de funciones necesarias
void registroDuplicado(void* elemOriginal, const void* elemEntrante){
    t_Registro* registroOriginal = (t_Registro*)elemOriginal;
    registroOriginal->apariciones++;
}

int cmp_tRegistro(const void* elem1, const void* elem2){
    t_Registro* registro1 = (t_Registro*)elem1;
    t_Registro* registro2 = (t_Registro*)elem2;
    return strcmp(registro1->texto,registro2->texto);
}

unsigned hash_tRegistro(const void* elem){
    t_Registro* registro = (t_Registro*)elem;
    return hash_str(registro->texto);
}

void mostrarRegistro(const void* elem){
    char buffer[MAX_BUFFER];
    t_Registro* registro = (t_Registro*)elem;
    formatearRegistro(buffer, registro->texto, registro->apariciones);
    printf("%s",buffer);
}

void mostrarHashMap(t_HashMap* hm){
    printAnimacion(HEADER);
    getchar();
    map_hash_map(hm, mostrarRegistro);
    printAnimacion("\n  Presione cualquier tecla para terminar...\n");
    getchar();
}


