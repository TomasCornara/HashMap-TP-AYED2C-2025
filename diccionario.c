#include "diccionario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// PROTOTIPOS DE FUNCIONES AUXILIARES
void destruir_elementos_lista(void* elem);
void destruir_elemento_dic(t_elemento* elem);
void crear_elemento_dic(t_elemento* elem, const char* clave, const void* valor, unsigned tamValor);
int comparar_elementos_dic(const void* elem1, const void* elem2);
void accion_duplicado_dic(void* elem_existente, const void* elem_nuevo);

/// FUNCIONES PRINCIPALES
void crear_dic(t_diccionario* dic, unsigned capacidad){
    dic->tabla = (tLista*)malloc(capacidad * sizeof(tLista));
    for(unsigned i = 0; i < capacidad; i++){
        crearLista(&dic->tabla[i]);
    }
    dic->capacidad = capacidad;
}

void destruir_dic(t_diccionario* dic){
    for(unsigned i = 0; i < dic->capacidad; i++){
        map_lista(&dic->tabla[i], destruir_elementos_lista);
        vaciarLista(&dic->tabla[i]);
    }
    free(dic->tabla);
}

void recorrer_dic(const t_diccionario* dic, accion_dic accion){
    for(unsigned i = 0; i < dic->capacidad; i++){
        if(!listaVacia(&dic->tabla[i])){
            map_lista(&dic->tabla[i], accion);
        }
    }
}

int poner_dic(t_diccionario* dic, const char* clave, const void* valor, unsigned tamValor, hash_func h_fun, comparar_func cmp_fun, duplicado_func dup_fun){
    unsigned hash = h_fun(clave) % dic->capacidad;

    t_elemento nuevo_elem;
    crear_elemento_dic(&nuevo_elem, clave, valor, tamValor);

    int resultado = ponerEnLista(&dic->tabla[hash], &nuevo_elem, sizeof(t_elemento),
                                cmp_fun, dup_fun);

    // Si no se pudo insertar, liberar memoria del elemento creado
    if(resultado != 1){
        destruir_elemento_dic(&nuevo_elem);
    }

    return resultado;
}

void* obtener_dic(const t_diccionario* dic, const char* clave, hash_func h_fun, comparar_func cmp_fun){
    unsigned hash = h_fun(clave) % dic->capacidad;

    // Crear elemento temporal para la búsqueda
    t_elemento elem_busqueda;
    elem_busqueda.clave = (char*)clave;
    elem_busqueda.valor = NULL;
    elem_busqueda.tamValor = 0;

    // Verificar si la lista no está vacía
    if(listaVacia(&dic->tabla[hash])){
        return NULL;
    }

    // Recorrer la lista manualmente accediendo directamente a los nodos
    tNodo* nodo_actual = dic->tabla[hash];
    
    while(nodo_actual){
        // Obtener el elemento almacenado en el nodo
        t_elemento* elemento_actual = (t_elemento*)(nodo_actual + 1);
        
        // Comparar con el elemento buscado
        if(cmp_fun(elemento_actual, &elem_busqueda) == 0){
            return elemento_actual->valor;
        }
        
        // Avanzar al siguiente nodo
        nodo_actual = nodo_actual->sig;
    }

    return NULL;
}



/// FUNCIONES AUXILIARES
void destruir_elementos_lista(void* elem){
    destruir_elemento_dic((t_elemento*)elem);
}

void crear_elemento_dic(t_elemento* elem, const char* clave, const void* valor, unsigned tamValor){
    elem->clave = (char*)malloc((strlen(clave) + 1) * sizeof(char));
    strcpy(elem->clave, clave);
    elem->valor = malloc(tamValor);
    memcpy(elem->valor, valor, tamValor);
    elem->tamValor = tamValor;
}

void destruir_elemento_dic(t_elemento* elem){
    free(elem->clave);
    free(elem->valor);
}

int comparar_elementos_dic(const void* elem1, const void* elem2){
    const t_elemento* e1 = (const t_elemento*)elem1;
    const t_elemento* e2 = (const t_elemento*)elem2;
    return strcmp(e1->clave, e2->clave);
}

void accion_duplicado_dic(void* elem_existente, const void* elem_nuevo){
    t_elemento* existente = (t_elemento*)elem_existente;
    const t_elemento* nuevo = (const t_elemento*)elem_nuevo;

    // Liberar el valor anterior
    free(existente->valor);

    // Actualizar con el nuevo valor
    existente->valor = malloc(nuevo->tamValor);
    memcpy(existente->valor, nuevo->valor, nuevo->tamValor);
    existente->tamValor = nuevo->tamValor;
}




