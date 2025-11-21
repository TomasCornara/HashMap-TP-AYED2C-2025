#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "diccionario.h"
#include "listaDinamica.h"
#include "parseArchivos.h"
#include "globales.h"
#include "menu.h"

#define SIZE_HS 50

///funciones para esta implementacion en particular
int cmp_tRegistro(const void* elem1, const void* elem2);
void registroDuplicado(void* elemOriginal, const void* elemEntrante);
void mostrarRegistro(void* elem, const t_diccionario* dic);
unsigned hashKR(const char *s);
void normalizar_minuscula(char *cadena);
unsigned contar_signos_puntuacion_dic(const t_diccionario* dic);
unsigned contar_palabras_dic(const t_diccionario* dic);

// Funciones del menú
void mostrarMenu();
int leerOpcion();
void mostrarEstadisticas(t_diccionario* dic);
void mostrarListadoPalabras(t_diccionario* dic);
void buscarPalabraEspecifica(t_diccionario* dic);
int procesarArchivo(const char* nombre_arch, t_diccionario* dic);
void limpiarBuffer();

///Programa principal
int main()
{
    ///BLOQUE DE DECLARACION
    char nombre_arch[256];
    t_diccionario dic;
    int opcion,
        len,
        archivo_procesado = 0;


    ///BLOQUE PRINCIPAL
    printAnimacion(HEADER);

    //Arch
    printf("  Ingrese el nombre del archivo de texto a procesar: ");

    if (fgets(nombre_arch, sizeof(nombre_arch), stdin) == NULL)
    {
        fprintf(stderr, "  Error: No se pudo leer el nombre del archivo\n");
        return -1;
    }
    // Eliminar el salto de línea
    len = strlen(nombre_arch);
    if (len > 0 && nombre_arch[len - 1] == '\n')
    {
        nombre_arch[len - 1] = '\0';
    }

    // Procesar el archivo
    if (procesarArchivo(nombre_arch, &dic) != 0)
    {
        return -1;
    }

    // Loop principal del menu
    do
    {
        mostrarMenu();
        opcion = leerOpcion();

        switch(opcion)
        {
        case 1:
            mostrarEstadisticas(&dic);
            break;
        case 2:
            mostrarListadoPalabras(&dic);
            break;
        case 3:
            buscarPalabraEspecifica(&dic);
            break;
        case 0:
            printf("\n  Programa terminado exitosamente.\n");
            break;
        default:
            printf("\n  Opcion invalida. Intente nuevamente.\n");
            break;
        }

        if (opcion != 0)
        {
            printf("\n  Presione Enter para continuar...");
            getchar();
        }

    }
    while (opcion != 0);

    ///BLOQUE LIMPIEZA
    if (archivo_procesado)
    {
        destruir_dic(&dic);
    }

    return 0;
}

///Implementacion de funciones auxiliares necesarias
void mostrarEstadisticas(t_diccionario* dic)
{
    unsigned total_palabras,
             total_elementos,
             signos_puntuacion,
             total_espacios,
             i;
    unsigned *ptr_espacios, *ptr_aux_punt;

    // Array de signos de puntuación a contar
    const char* signos_puntuacion_array[] = {
        ".", ",", "!", "?", ";", ":", "-", "(", ")",
        "[", "]", "{", "}", "'", "\"", "¿", "¡", NULL
    };

    system("cls");
    printf("\n");
    printf("  ------------------------------------------\n");
    printf("  |        ESTADISTICAS GENERALES          |\n");
    printf("  ------------------------------------------\n");
    printf("\n");


    ///Calculos

    //Espacios
    ptr_espacios = (unsigned*)obtener_dic(dic, " ");
    total_espacios = (ptr_espacios != NULL) ? *ptr_espacios : 0;

    //Signos puntuacion
    signos_puntuacion = 0;
    for(i = 0; signos_puntuacion_array[i] != NULL; i++)
    {
        ptr_aux_punt = (unsigned*)obtener_dic(dic, signos_puntuacion_array[i]);
        if(ptr_aux_punt)
        {
            signos_puntuacion += *ptr_aux_punt;
        }
    }


    //Palabras
    total_palabras = contar_palabras_dic(dic);

    //Total elementos
    total_elementos = 0;
    for(i = 0; i < dic->capacidad; i++)
    {
        total_elementos += contarElementosLista(&dic->tabla[i]);
    }

    //Print
    printf("  Cantidad total de palabras unicas: %u\n", total_palabras);
    printf("  Cantidad de signos de puntuacion: %u\n", signos_puntuacion);
    printf("  Total de espacios: %u\n", total_espacios);
    printf("  Total elementos unicos en el diccionario: %u\n", total_elementos);
    printf("  Capacidad del diccionario: %u\n", dic->capacidad);
    printf("\n");
}

void mostrarListadoPalabras(t_diccionario* dic)
{
    system("cls");
    printf("\n");
    printf("  ------------------------------------------\n");
    printf("  |      LISTADO DE APARICIONES            |\n");
    printf("  ------------------------------------------\n");
    printf("\n");
    recorrer_dic(dic, mostrarRegistro);
    printf("\n");
}

void buscarPalabraEspecifica(t_diccionario* dic)
{
    char palabra_buscar[MAX_BUFFER];
    size_t len;
    unsigned* apariciones_encontradas;
    unsigned hash_valor;

    system("cls");
    printf("\n");
    printf("  ------------------------------------------\n");
    printf("  |       BUSQUEDA DE PALABRA               |\n");
    printf("  ------------------------------------------\n");
    printf("\n");

    printf("  Ingrese la palabra a buscar: ");

    if (fgets(palabra_buscar, sizeof(palabra_buscar), stdin) == NULL)
    {
        printf("  Error al leer la palabra.\n");
        return;
    }

    // Cambiar el salto de linea
    len = strlen(palabra_buscar);
    if (len > 0 && palabra_buscar[len - 1] == '\n')
    {
        palabra_buscar[len - 1] = '\0';
    }

    // Normalizar a min
    normalizar_minuscula(palabra_buscar);

    // Buscar la palabra en el diccionario
    apariciones_encontradas = obtener_dic(dic, palabra_buscar);

    if (apariciones_encontradas)
    {
        hash_valor = hashKR(palabra_buscar) % dic->capacidad;
        printf("   Palabra: %s\n", palabra_buscar);
        printf("   Apariciones: %u\n", *apariciones_encontradas);
        printf("   Valor hash: %u\n", hash_valor);
    }
    else
    {
        printf("   '%s' no fue encontrada en el diccionario.\n", palabra_buscar);
    }
    printf("\n");
}

int procesarArchivo(const char* nombre_arch, t_diccionario* dic)
{
    FILE* arch;
    char texto_buffer[MAX_BUFFER];
    unsigned apariciones;
    unsigned palabras_procesadas;

    // Apertura del archivo
    if (!(arch = abrirArchivoTextoL((char*)nombre_arch)))
    {
        fprintf(stderr, "  Error: No se pudo abrir el archivo '%s'\n", nombre_arch);
        return -1;
    }

    // Creacion del diccionario
    if(!crear_dic(dic, SIZE_HS)){
        fclose(arch);
        fprintf(stderr, "  Error: No se pudo crear el diccionario\n");
        return 0;
    };

    printf("  Leyendo y procesando palabras");

    // Procesar archivo palabra por palabra
    palabras_procesadas = 0;
    while (sigPalArch(arch, texto_buffer, MAX_BUFFER))
    {
        unsigned* apariciones_existentes;

        normalizar_minuscula(texto_buffer);
        // Verificar si la palabra ya existe
        apariciones_existentes = (unsigned*)obtener_dic(dic, texto_buffer);
        if (apariciones_existentes)
        {
            apariciones = (*apariciones_existentes) + 1;
        }
        else
        {
            // Palabra nueva
            apariciones = 1;
        }

        poner_dic(dic, texto_buffer, &apariciones, sizeof(unsigned));

        palabras_procesadas++;
        if (palabras_procesadas % 100 == 0)
        {
            fflush(stdout);
        }
    }

    printf("\n  Palabras procesadas: %u\n", palabras_procesadas);

    fclose(arch);
    return 0;
}

void limpiarBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void normalizar_minuscula(char *cadena)
{
    unsigned i;

    if (cadena == NULL) return;

    for (i = 0; cadena[i] != '\0'; i++)
    {
        cadena[i] = (char)tolower((unsigned char)cadena[i]);
    }
}

void registroDuplicado(void* elemOriginal, const void* elemEntrante)
{
    t_elemento* elemento_existente;
    elemento_existente = (t_elemento*)elemOriginal;
    (*(unsigned*)elemento_existente->valor)++;
}

int cmp_tRegistro(const void* elem1, const void* elem2)
{
    const t_elemento* elemento1;
    const t_elemento* elemento2;

    elemento1 = (const t_elemento*)elem1;
    elemento2 = (const t_elemento*)elem2;

    return strcmp(elemento1->clave, elemento2->clave);
}

void mostrarRegistro(void* elem, const t_diccionario* dic)
{
    char buffer[MAX_BUFFER];
    t_elemento* elemento;
    unsigned* apariciones;
    unsigned hash_valor;

    elemento = (t_elemento*)elem;
    apariciones = (unsigned*)elemento->valor;
    hash_valor = dic->hash_fn(elemento->clave) % dic->capacidad;

    formatearRegistro(buffer, elemento->clave, *apariciones, hash_valor);
    printf("%s", buffer);
}

unsigned contar_signos_puntuacion_dic(const t_diccionario* dic)
{
    unsigned i;
    unsigned total_signos = 0;

    for(i = 0; i < dic->capacidad; i++)
    {
        if(!listaVacia(&dic->tabla[i]))
        {
            tLista nodo_actual = dic->tabla[i];

            while(nodo_actual)
            {
                t_elemento* elemento_actual = (t_elemento*) nodo_actual->dato;

                if(es_signo_puntuacion(elemento_actual->clave))
                {
                    total_signos++;
                }
                nodo_actual = nodo_actual->sig;
            }
        }
    }

    return total_signos;
}

unsigned contar_palabras_dic(const t_diccionario* dic)
{
    unsigned i;
    unsigned total_palabras = 0;

    for(i = 0; i < dic->capacidad; i++)
    {
        if(!listaVacia(&dic->tabla[i]))
        {
            tLista nodo_actual = dic->tabla[i];

            while(nodo_actual)
            {
                t_elemento* elemento_actual = (t_elemento*) nodo_actual->dato;

                if(!es_signo_puntuacion(elemento_actual->clave))
                {
                    total_palabras++;
                }
                nodo_actual = nodo_actual->sig;
            }
        }
    }

    return total_palabras;
}



