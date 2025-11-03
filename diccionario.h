#ifndef DICCIONARIO_H_INCLUDED
#define DICCIONARIO_H_INCLUDED

#include "listaDinamica.h"

typedef struct
{
    char* clave;
    void* valor;
    unsigned tamValor;
} t_elemento;

typedef struct
{
    tLista* tabla;
    unsigned capacidad;
} t_diccionario;

typedef void (*accion_dic)(void* valor);
typedef unsigned (*hash_func)(const char* clave);
typedef int (*comparar_func)(const void* elem1, const void* elem2);
typedef void (*duplicado_func)(void* elem_existente, const void* elem_nuevo);

void crear_dic(t_diccionario* dic, unsigned capacidad);
void destruir_dic(t_diccionario* dic);
void recorrer_dic(const t_diccionario* dic, accion_dic accion);
int poner_dic(t_diccionario* dic, const char* clave, const void* valor, unsigned tamValor, hash_func h_fun, comparar_func cmp_fun, duplicado_func dup_fun);
void* obtener_dic(const t_diccionario* dic, const char* clave, hash_func h_fun, comparar_func cmp_fun);
int sacar_dic(t_diccionario* dic, const char* clave, hash_func h_fun, comparar_func cmp_fun);
unsigned contar_palabras_dic(const t_diccionario* dic);


#endif // DICCIONARIO_H_INCLUDED
