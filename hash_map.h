#ifndef HASH_MAP_H_INCLUDED
#define HASH_MAP_H_INCLUDED

#include "listaDinamica.h"

#define MAX_VEC 500


typedef struct{
    tLista vec[MAX_VEC];
}t_HashMap;

typedef void (*f_Map)(const void* elem);
typedef int f_cmp(const void* elem1, const void* elem2);
typedef unsigned(*f_hash)(const void* elem);


void crear_hash_map(t_HashMap* hm);
int poner_en_hmap(t_HashMap* hm,const void* elem,unsigned cantBytes,f_hash hash_func, f_cmp cmp);
void map_hash_map(t_HashMap* hm, f_Map func);
unsigned hash_str(const void* elem);

#endif // HASH_MAP_H_INCLUDED
