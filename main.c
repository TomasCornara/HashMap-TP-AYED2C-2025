#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_map.h"
#include "listaDinamica.h"

void print_nombre(const void* elem){
    char* e = (char*)elem;
    printf("%s",e);
}

void funcion_map_prueba(const void* elem){
    tLista* lista = (tLista*)elem;
    map_lista(lista,print_nombre);
}

unsigned hash_str(const void* elem) {
    const char* s = (const char*)elem;
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % MAX_VEC;
}

int cmp_strings(const void* elem1, const void* elem2){
    return strcmp((const char*)elem1,(const char*)elem2);
}

int main()
{
    t_HashMap mi_diccionario;
    char *nombres[] = {"Ana", "Luis", "Carlos", "Marta", "Sofia"};
    int n = sizeof(nombres) / sizeof(nombres[0]);


    crear_hash_map(&mi_diccionario);

    for(int i = 0; i < n; i++){
        poner_en_hmap(&mi_diccionario,nombres[i],strlen(nombres[i])+1,hash_str,cmp_strings);
    }

    // Mostrar el contenido del hash map
    printf("Contenido del HashMap:\n");
    map_hash_map(&mi_diccionario, funcion_map_prueba);


    return 0;
}
