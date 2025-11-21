#ifndef DICCIONARIO_H_INCLUDED
#define DICCIONARIO_H_INCLUDED

#include "listaDinamica.h"

typedef struct
{
    char* clave;
    void* valor;
    unsigned tamValor;
} t_elemento;

typedef struct s_diccionario t_diccionario;

typedef unsigned (*hash_func)(const char* clave);
typedef int (*comparar_func)(const char* elem1, const char* elem2);
typedef void (*duplicado_func)(void* elem_existente, const void* elem_nuevo);
typedef void (*accion_dic)(void* valor, const t_diccionario* dic);

struct s_diccionario
{
    tLista* tabla;
    unsigned capacidad;
    hash_func hash_fn;
    comparar_func cmp_fn;
    duplicado_func dup_fn;
};

int crear_dic(t_diccionario* dic, unsigned capacidad);
void destruir_dic(t_diccionario* dic);
void recorrer_dic(const t_diccionario* dic, accion_dic accion);
int poner_dic(t_diccionario* dic, const char* clave, const void* valor, unsigned tamValor);
void* obtener_dic(const t_diccionario* dic, const char* clave);
int sacar_dic(t_diccionario* dic, const char* clave);


unsigned hash_dic(const char* s);
unsigned hashKR(const char *s);

#endif // DICCIONARIO_H_INCLUDED

