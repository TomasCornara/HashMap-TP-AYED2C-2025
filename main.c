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

unsigned hash_tRegistro(const char* clave);
int cmp_tRegistro(const void* elem1, const void* elem2);
void registroDuplicado(void* elemOriginal, const void* elemEntrante);
void mostrarRegistro(void* elem);
void mostrarHashMap(t_diccionario* hm);
unsigned hashKR(const char *s);
void normalizar_minuscula(char *cadena);

///Programa
int main(int argc, char *argv[])
{
    ///Bloque declarativo
    char nombre_arch[256];
    unsigned len;
    FILE* arch;
    t_diccionario dic;
    t_Registro buffer_registro;

    ///Menu
    printAnimacion(HEADER);
    getc(stdin);

    ///Bloque ingreso de datos
    printf("Ingrese el nombre del archivo: ");
    if (fgets(nombre_arch, sizeof(nombre_arch), stdin) == NULL) {
        fprintf(stderr, "Error: No se pudo leer el nombre del archivo\n");
        return -1;
    }

    // Eliminar el salto de línea
    len = strlen(nombre_arch);
    if (len > 0 && nombre_arch[len - 1] == '\n') {
        nombre_arch[len - 1] = '\0';
    }


    ///Bloque inicializacion
    if(!(arch = abrirArchivoTextoL(nombre_arch))){
        fprintf(stderr,"Error: No se pudo abrir correctamente el archivo");
        return -2;
    }
    crear_dic(&dic,SIZE_HS);

    ///Bloque proceso
    while(sigPalArch(arch,buffer_registro.texto,MAX_BUFFER)){
        buffer_registro.apariciones = 1;  // Inicializar apariciones
        normalizar_minuscula(buffer_registro.texto); //Normalizacion del texto a minus
        poner_dic(&dic, buffer_registro.texto, &buffer_registro, sizeof(t_Registro), hash_tRegistro, cmp_tRegistro, registroDuplicado); //Poner en diccionario
    }

    ///Mostrar resultados
    recorrer_dic(&dic,mostrarRegistro);

    ///Prueba de obtener_dic
    printf("\nPrueba de obtener_dic\n");
    t_Registro* registro_encontrado = (t_Registro*)obtener_dic(&dic, "lorem", hash_tRegistro, cmp_tRegistro);
    if(registro_encontrado){
        printf("Palabra 'lorem' encontrada: %u apariciones\n", registro_encontrado->apariciones);
    } else {
        printf("Palabra 'lorem' no encontrada en el diccionario\n");
    }

    ///Prueba de sacar_dic
    printf("\nPrueba de sacar_dic\n");
    printf("Intentando eliminar la palabra 'lorem'...\n");
    if(sacar_dic(&dic, "lorem", hash_tRegistro, cmp_tRegistro)){
        printf("Palabra 'lorem' eliminada exitosamente\n");

        // Verificar que ya no existe
        registro_encontrado = (t_Registro*)obtener_dic(&dic, "lorem", hash_tRegistro, cmp_tRegistro);
        if(!registro_encontrado){
            printf("Confirmado: la palabra 'lorem' ya no existe en el diccionario\n");
        }
    } else {
        printf("No se pudo eliminar la palabra 'lorem' (posiblemente no existe)\n");
    }

    ///Bloque limpieza
    destruir_dic(&dic);
    fclose(arch);


    return 0;
}

///Implementacion de funciones auxiliares necesarias
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
    formatearRegistro(buffer, registro->texto, registro->apariciones,hashKR(elemento->clave) % SIZE_HS);
    printf("%s",buffer);
}

void mostrarHashMap(t_diccionario* hm){
    printAnimacion(HEADER);
    recorrer_dic(hm, mostrarRegistro);
    printAnimacion("\n  Presione cualquier tecla para terminar...\n");
    getchar();
}


