#include "hash_map.h"
#include <stdio.h>


void crear_hash_map(t_HashMap* hm){
    for(int i = 0; i < MAX_VEC; i++){
        crearLista(&hm->vec[i]);
    }
}


void map_hash_map(t_HashMap* hm, f_Map func){
    for(int i = 0; i < MAX_VEC; i++){
        printf("%d-",i); //ESTO REALMENTE NO VA (Solo debug)
        func(&hm->vec[i]);
        printf("\n"); //Y ESTO TAMPOCO
    }
}

int poner_en_hmap(t_HashMap* hm,const void* elem,unsigned cantBytes,f_hash hash_func, f_cmp cmp){
    unsigned hash = hash_func(elem);
    return ponerEnLista(&hm->vec[hash],elem,cantBytes,cmp);

}


unsigned hash_str(const void* elem) {
    const char* s = (const char*)elem;
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % MAX_VEC;
}
