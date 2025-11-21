#include "diccionario.h"
#include "parseArchivos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// PROTOTIPOS DE FUNCIONES AUXILIARES
void destruir_elementos_lista(void* elem);
void destruir_elemento_dic(t_elemento* elem);
void crear_elemento_dic(t_elemento* elem, const char* clave, const void* valor, unsigned tamValor);
int comparar_elementos_dic(const void* elem1, const void* elem2);
void accion_duplicado_dic(void* elem_existente, const void* elem_nuevo);
int wrapper_comparar_elementos(const void* elem1, const void* elem2);

/// FUNCIONES PRINCIPALES
int crear_dic(t_diccionario* dic, unsigned capacidad)
{
    unsigned i;

    dic->tabla = (tLista*)malloc(capacidad * sizeof(tLista));
    if(!dic->tabla){
        return 0;
    }

    for(i = 0; i < capacidad; i++)
    {
        crearLista(&dic->tabla[i]);
    }
    dic->capacidad = capacidad;
    dic->hash_fn = hashKR;
    dic->cmp_fn = strcmp;
    dic->dup_fn = accion_duplicado_dic;
    return 1;
}

void destruir_dic(t_diccionario* dic)
{
    unsigned i;
    tLista nodo_actual;

    for(i = 0; i < dic->capacidad; i++)
    {
        nodo_actual = dic->tabla[i];
        while(nodo_actual)
        {
            destruir_elementos_lista(nodo_actual->dato);
            nodo_actual = nodo_actual->sig;
        }
        vaciarLista(&dic->tabla[i]);
    }
    free(dic->tabla);
}

void recorrer_dic(const t_diccionario* dic, accion_dic accion)
{
    unsigned i;
    tLista nodo_actual;

    for(i = 0; i < dic->capacidad; i++)
    {
        if(!listaVacia(&dic->tabla[i]))
        {
            nodo_actual = dic->tabla[i];
            while(nodo_actual)
            {
                accion(nodo_actual->dato, dic);
                nodo_actual = nodo_actual->sig;
            }
        }
    }
}

int poner_dic(t_diccionario* dic, const char* clave, const void* valor, unsigned tamValor)
{
    int resultado;
    unsigned hash = dic->hash_fn(clave) % dic->capacidad;
    t_elemento nuevo_elem;


    crear_elemento_dic(&nuevo_elem, clave, valor, tamValor);

    resultado = ponerEnLista(&dic->tabla[hash], &nuevo_elem, sizeof(t_elemento),
                                 wrapper_comparar_elementos, dic->dup_fn);

    if(resultado != 1)
    {
        destruir_elemento_dic(&nuevo_elem);
    }

    return resultado;
}

void* obtener_dic(const t_diccionario* dic, const char* clave)
{
    unsigned hash;
    tNodo* nodo_encontrado;
    t_elemento elem_busqueda;
    t_elemento* elemento_encontrado;

    hash = dic->hash_fn(clave) % dic->capacidad;

    elem_busqueda.clave = (char*)clave;
    elem_busqueda.valor = NULL;
    elem_busqueda.tamValor = 0;

    nodo_encontrado = buscarEnLista(&dic->tabla[hash], &elem_busqueda, wrapper_comparar_elementos);

    if (nodo_encontrado == NULL)
    {
        return NULL;
    }

    elemento_encontrado = (t_elemento*)nodo_encontrado->dato;
    return elemento_encontrado->valor;
}

int sacar_dic(t_diccionario* dic, const char* clave)
{
    unsigned hash;

    hash = dic->hash_fn(clave) % dic->capacidad;

    t_elemento elem_busqueda;
    elem_busqueda.clave = (char*)clave;
    elem_busqueda.valor = NULL;
    elem_busqueda.tamValor = 0;

    if(listaVacia(&dic->tabla[hash]))
    {
        return 0;
    }

    return sacarDeLista(&dic->tabla[hash], &elem_busqueda, wrapper_comparar_elementos, destruir_elementos_lista);
}

/// FUNCIONES AUXILIARES
void destruir_elementos_lista(void* elem)
{
    destruir_elemento_dic((t_elemento*)elem);
}

void crear_elemento_dic(t_elemento* elem, const char* clave, const void* valor, unsigned tamValor)
{
    elem->clave = (char*)malloc((strlen(clave) + 1) * sizeof(char));
    strcpy(elem->clave, clave);
    elem->valor = malloc(tamValor);
    memcpy(elem->valor, valor, tamValor);
    elem->tamValor = tamValor;
}

void destruir_elemento_dic(t_elemento* elem)
{
    free(elem->clave);
    free(elem->valor);
}

int comparar_elementos_dic(const void* elem1, const void* elem2)
{
    const t_elemento* e1;
    const t_elemento* e2;

    e1 = (const t_elemento*)elem1;
    e2 = (const t_elemento*)elem2;

    return strcmp(e1->clave, e2->clave);
}

void accion_duplicado_dic(void* elem_existente, const void* elem_nuevo)
{
    t_elemento* existente;
    const t_elemento* nuevo;

    existente = (t_elemento*)elem_existente;
    nuevo = (const t_elemento*)elem_nuevo;

    free(existente->valor);

    existente->valor = malloc(nuevo->tamValor);
    memcpy(existente->valor, nuevo->valor, nuevo->tamValor);
    existente->tamValor = nuevo->tamValor;
}

unsigned hashKR(const char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval;
}

int wrapper_comparar_elementos(const void* elem1, const void* elem2)
{
    return comparar_elementos_dic(elem1, elem2);
}
