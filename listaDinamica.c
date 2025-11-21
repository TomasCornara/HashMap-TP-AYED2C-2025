#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "listaDinamica.h"

void crearLista(tLista* lista)
{
    *lista = NULL;
}

int listaVacia(const tLista* lista)
{
    return *lista == NULL;
}

int listaLlena(const tLista* lista, unsigned cantBytes)
{
    tNodo* aux = malloc(sizeof(tNodo) + cantBytes);
    if(aux == NULL)
        return 1;

    free(aux);
    return 0;
}

int ponerPrimero(tLista* lista, const void* dato, unsigned cantBytes)
{
    tNodo* nue;

    nue = malloc(sizeof(tNodo) + cantBytes);
    if(!nue)
    {
        return 0;
    }

    nue->dato=nue+1;
    nue->sig = *lista;
    nue->tam = cantBytes;
    memcpy(nue->dato,dato,cantBytes);
    *lista = nue;

    return 1;
}

void vaciarLista(tLista* lista)
{
    tNodo* elim;

    while(*lista)
    {
        elim = *lista;
        *lista = elim->sig;
        free(elim);
    }
}

int ponerEnLista(tLista* lista, const void* dato, unsigned cantBytes,
                 int (*comparar)(const void*, const void*),
                 void (*accionDuplicado)(void*, const void*))
{
    tNodo* nue;
    tNodo** p_lista = lista;

    // Validaci�n de parametros
    if (!lista || !dato || !comparar)
        return 0;

    // Buscar posicion ordenada
    while (*p_lista && comparar(dato, (*p_lista)->dato) > 0)
        p_lista = &((*p_lista)->sig);

    // Si ya existe el elemento
    if (*p_lista && comparar(dato, (*p_lista)->dato) == 0)
    {
        if (!accionDuplicado)
            return 0;
        accionDuplicado((*p_lista)->dato, dato);
        return 1;
    }

    // Crear nuevo nodo
    nue = malloc(sizeof(tNodo) + cantBytes);
    if (!nue)
        return 0;

    nue->dato=nue+1;
    memcpy(nue->dato, dato, cantBytes);
    nue->tam = cantBytes;
    nue->sig = *p_lista;
    *p_lista = nue;

    return 1;
}


void map_lista(const tLista* lista,void print(void* elem))
{
    while(*lista)
    {
        print((*lista)->dato);
        lista = &(*lista)->sig;
    }
}

int sacarDeLista(tLista* lista, const void* dato,
                 int (*comparar)(const void*, const void*),
                 void (*destruir)(void*))
{
    tNodo** p_lista = lista;
    tNodo* nodo_a_eliminar;

    // Validación de parámetros
    if (!lista || !dato || !comparar)
        return 0;

    // Buscar el elemento
    while (*p_lista && comparar(dato, (*p_lista)->dato) != 0)
        p_lista = &((*p_lista)->sig);

    // Si no se encontró el elemento
    if (!*p_lista)
        return 0;

    // Guardar referencia al nodo a eliminar
    nodo_a_eliminar = *p_lista;

    // Llamar a la función de destrucción si se proporcionó
    if (destruir)
        destruir(nodo_a_eliminar->dato);

    // Desenlazar el nodo de la lista
    *p_lista = nodo_a_eliminar->sig;

    // Liberar memoria del nodo
    free(nodo_a_eliminar);

    return 1;
}

unsigned contarElementosLista(const tLista* lista)
{
    unsigned contador = 0;
    tLista nodo_actual = *lista;

    while (nodo_actual)
    {
        contador++;
        nodo_actual = nodo_actual->sig;
    }

    return contador;
}

tNodo* buscarEnLista(const tLista* lista, const void* dato,
                     int (*comparar)(const void*, const void*))
{
    tNodo* nodo_actual;

    if (!lista || !dato || !comparar)
        return NULL;

    if (!*lista)
        return NULL;

    nodo_actual = *lista;

    while (nodo_actual)
    {
        if (comparar(dato, nodo_actual->dato) == 0)
        {
            return nodo_actual;
        }
        nodo_actual = nodo_actual->sig;
    }

    return NULL;
}
