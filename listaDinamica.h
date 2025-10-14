#ifndef LISTADINAMICA_H_INCLUDED
#define LISTADINAMICA_H_INCLUDED

typedef struct sNodo{
    unsigned tam;
    struct sNodo* sig;
}tNodo;

typedef tNodo* tLista;

void crearLista(tLista* lista);
int listaVacia(const tLista* lista);
int listaLlena(const tLista* lista, unsigned cantBytes);
int ponerPrimero(tLista* lista, const void* dato, unsigned cantBytes);
void vaciarLista(tLista* lista);
int ponerEnLista(tLista* lista, const void* dato, unsigned cantBytes, int comparar(const void*,const void*));
void map_lista(const tLista* lista,void print(const void* elem));

#endif // LISTADINAMICA_H_INCLUDED
