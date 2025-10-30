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
void map_lista(const tLista* lista,void print(void* elem));
int ponerEnLista(tLista* lista, const void* dato, unsigned cantBytes,
                 int (*comparar)(const void*, const void*),
                 void (*accionDuplicado)(void*, const void*));

#endif // LISTADINAMICA_H_INCLUDED
