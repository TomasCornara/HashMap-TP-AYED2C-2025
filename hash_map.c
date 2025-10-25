#include "hash_map.h"
#include <stdio.h>


void crear_hash_map(t_HashMap* hm){
    for(int i = 0; i < MAX_VEC; i++){
        crearLista(&hm->vec[i]);
    }
}


void map_hash_map(t_HashMap* hm, f_Map func){
    for(int i = 0; i < MAX_VEC; i++){
        if(!listaVacia(&hm->vec[i])){
            map_lista(&hm->vec[i], func);
        }
    }
}

int poner_en_hmap(t_HashMap* hm,const void* elem,unsigned cantBytes,f_hash hash_func, f_cmp cmp, f_accion_dup accion_dup){
    unsigned hash = hash_func(elem);
    return ponerEnLista(&hm->vec[hash],elem,cantBytes,cmp,accion_dup);

}


unsigned hash_str(const void* elem) {
    const char* s = (const char*)elem;
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % MAX_VEC;
}

void vaciar_hash_map(t_HashMap* hm){
    for(int i = 0; i < MAX_VEC; i++){
        vaciarLista(&hm->vec[i]);
    }
}
