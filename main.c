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
typedef struct {
    char texto[MAX_BUFFER];
    unsigned apariciones;
}t_Registro;

// Prototipos de funciones
unsigned hash_tRegistro(const char* clave);
int cmp_tRegistro(const void* elem1, const void* elem2);
void registroDuplicado(void* elemOriginal, const void* elemEntrante);
void mostrarRegistro(void* elem);
void mostrarHashMap(t_diccionario* hm);
unsigned hashKR(const char *s);
void normalizar_minuscula(char *cadena);

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

    if (fgets(nombre_arch, sizeof(nombre_arch), stdin) == NULL) {
        fprintf(stderr, "  Error: No se pudo leer el nombre del archivo\n");
        return -1;
    }
    // Eliminar el salto de línea
    len = strlen(nombre_arch);
    if (len > 0 && nombre_arch[len - 1] == '\n') {
        nombre_arch[len - 1] = '\0';
    }

    // Procesar el archivo
    if (procesarArchivo(nombre_arch, &dic) != 0) {
        return -1;
    }

    // Loop principal del menú
    do {
        mostrarMenu();
        opcion = leerOpcion();

        switch(opcion) {
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

        if (opcion != 0) {
            printf("\n  Presione Enter para continuar...");
            getchar();
        }

    } while (opcion != 0);

    ///BLOQUE LIMPIEZA
    if (archivo_procesado) {
        destruir_dic(&dic);
    }

    return 0;
}

///Implementacion de funciones auxiliares necesarias
void mostrarEstadisticas(t_diccionario* dic) {
    unsigned total_palabras,
             total_elementos,
             signos_puntuacion,
             i;

    system("cls");
    printf("\n");
    printf("  ------------------------------------------\n");
    printf("  |        ESTADISTICAS GENERALES          |\n");
    printf("  ------------------------------------------\n");
    printf("\n");


    //Calculos
    total_palabras = contar_palabras_dic(dic);
    total_elementos = 0;
    for(i = 0; i < dic->capacidad; i++){
        total_elementos += contarElementosLista(&dic->tabla[i]);
    }
    signos_puntuacion = total_elementos - total_palabras;

    //Print
    printf("  Cantidad total de palabras unicas: %u\n", total_palabras);
    printf("  Cantidad de signos de puntuacion: %u\n", signos_puntuacion);
    printf("  Total de elementos procesados: %u\n", total_elementos);
    printf("  Capacidad del diccionario: %u\n", dic->capacidad);
    printf("\n");
}

void mostrarListadoPalabras(t_diccionario* dic) {
    system("cls");
    printf("\n");
    printf("  ------------------------------------------\n");
    printf("  |      LISTADO DE APARICIONES            |\n");
    printf("  ------------------------------------------\n");
    printf("\n");
    recorrer_dic(dic, mostrarRegistro);
    printf("\n");
}

void buscarPalabraEspecifica(t_diccionario* dic) {
    char palabra_buscar[MAX_BUFFER];
    size_t len;
    t_Registro* registro_encontrado;
    unsigned hash_valor;

    system("cls");
    printf("\n");
    printf("  ------------------------------------------\n");
    printf("  |       BUSQUEDA DE PALABRA               |\n");
    printf("  ------------------------------------------\n");
    printf("\n");

    printf("  Ingrese la palabra a buscar: ");

    if (fgets(palabra_buscar, sizeof(palabra_buscar), stdin) == NULL) {
        printf("  Error al leer la palabra.\n");
        return;
    }

    // Eliminar el salto de línea
    len = strlen(palabra_buscar);
    if (len > 0 && palabra_buscar[len - 1] == '\n') {
        palabra_buscar[len - 1] = '\0';
    }

    // Normalizar a min
    normalizar_minuscula(palabra_buscar);

    // Buscar la palabra en el diccionario
    registro_encontrado = obtener_dic(dic, palabra_buscar, hash_tRegistro, cmp_tRegistro);

    if (registro_encontrado) {
        hash_valor = hash_tRegistro(palabra_buscar) % dic->capacidad;
        printf("   Palabra: %s\n", registro_encontrado->texto);
        printf("   Apariciones: %u\n", registro_encontrado->apariciones);
        printf("   Valor hash: %u\n", hash_valor);
    } else {
        printf("   '%s' no fue encontrada en el diccionario.\n", palabra_buscar);
    }
    printf("\n");
}

int procesarArchivo(const char* nombre_arch, t_diccionario* dic) {
    FILE* arch;
    t_Registro buffer_registro;
    unsigned palabras_procesadas;

    // Abrir archivo
    if (!(arch = abrirArchivoTextoL((char*)nombre_arch))) {
        fprintf(stderr, "  Error: No se pudo abrir el archivo '%s'\n", nombre_arch);
        return -1;
    }

    // Crear diccionario
    crear_dic(dic, SIZE_HS);

    printf("  Leyendo y procesando palabras");

    // Procesar archivo palabra por palabra
    palabras_procesadas = 0;
    while (sigPalArch(arch, buffer_registro.texto, MAX_BUFFER)) {
        buffer_registro.apariciones = 1;
        normalizar_minuscula(buffer_registro.texto);
        poner_dic(dic, buffer_registro.texto, &buffer_registro, sizeof(t_Registro),
                 hash_tRegistro, cmp_tRegistro, registroDuplicado);

        palabras_procesadas++;
        if (palabras_procesadas % 100 == 0) {
            printf(".");
            fflush(stdout);
        }
    }

    printf("\n  Palabras procesadas: %u\n", palabras_procesadas);

    fclose(arch);
    return 0;
}

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void normalizar_minuscula(char *cadena) {
    if (cadena == NULL) return;

    for (size_t i = 0; cadena[i] != '\0'; i++) {
        cadena[i] = (char)tolower((unsigned char)cadena[i]);
    }
}

unsigned hashKR(const char *s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval;
}

void registroDuplicado(void* elemOriginal, const void* elemEntrante){
    t_elemento* elemento_existente = (t_elemento*)elemOriginal;
    t_Registro* registroOriginal = (t_Registro*)elemento_existente->valor;
    registroOriginal->apariciones++;
}

int cmp_tRegistro(const void* elem1, const void* elem2){
    const t_elemento* elemento1 = (const t_elemento*)elem1;
    const t_elemento* elemento2 = (const t_elemento*)elem2;
    return strcmp(elemento1->clave, elemento2->clave);
}

unsigned hash_tRegistro(const char* clave){
    return hashKR(clave);
}

void mostrarRegistro(void* elem){
    char buffer[MAX_BUFFER];
    t_elemento* elemento = (t_elemento*)elem;
    t_Registro* registro = (t_Registro*)elemento->valor;
    unsigned hash_valor = hash_tRegistro(elemento->clave) % SIZE_HS;
    formatearRegistro(buffer, registro->texto, registro->apariciones, hash_valor);
    printf("%s",buffer);
}

void mostrarHashMap(t_diccionario* hm){
    printAnimacion(HEADER);
    recorrer_dic(hm, mostrarRegistro);
    printAnimacion("\n  Presione cualquier tecla para terminar...\n");
    getchar();
}


