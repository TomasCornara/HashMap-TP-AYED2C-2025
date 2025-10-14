#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "listaDinamica.h"

void crearLista(tLista* lista){
    *lista = NULL;
}

int listaVacia(const tLista* lista){
    return *lista == NULL;
}

int listaLlena(const tLista* lista, unsigned cantBytes) {
    tNodo* aux = malloc(sizeof(tNodo) + cantBytes);
    if(aux == NULL)
        return 1;

    free(aux);
    return 0;
}

int ponerPrimero(tLista* lista, const void* dato, unsigned cantBytes){
    tNodo* nue;

    nue = malloc(sizeof(tNodo) + cantBytes);
    if(!nue){
        return 0;
    }

    nue->sig = *lista;
    nue->tam = cantBytes;
    memcpy(nue + 1,dato,cantBytes);
    *lista = nue;

    return 1;
}

void vaciarLista(tLista* lista){
    tNodo* elim;

    while(*lista){
        elim = *lista;
        *lista = elim->sig;
        free(elim);
    }
}

int ponerEnLista(tLista* lista, const void* dato, unsigned cantBytes, int comparar(const void*,const void*)){
    tNodo* nue;
    tNodo** p_lista = lista; // Usar un puntero auxiliar para recorrer

    if(!(nue = malloc(sizeof(tNodo) + cantBytes))){
        return 0;
    }

    //Carga el nodo
    memcpy(nue + 1,dato,cantBytes);
    nue->tam = cantBytes;

    //Busca posicion
    while(*p_lista && comparar(dato,(*p_lista) + 1) > 0){
        p_lista = &((*p_lista)->sig);
    }

    nue->sig = *p_lista;
    *p_lista = nue;


    return 1;
}

void map_lista(const tLista* lista,void print(const void* elem)){
    while(*lista){
        print((*lista) + 1);
        lista = &(*lista)->sig;
    }
}
