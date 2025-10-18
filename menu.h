#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#define HEADER \
        "                                                                                        \n"\
        "  --------------------------------------------------------------------------------------\n"\
        "  | TITULO: DICCIONARIO DE DATOS            || ALUMNOS:                                |\n"\
        "  | TIPO: TRABAJO PRACTICO                  ||   - PENELA SANTIAGO                     |\n"\
        "  | PROFESORES:                             ||   - VALENTIN NIEVAS                     |\n"\
        "  |   - PEZZOLA, FEDERICO DANIEL            ||   - FACUNDO CABALLERO                   |\n"\
        "  |   - JORDI, BRIAN EDUARDO                ||   - TOMAS ANDRES CORNARA PEREZ          |\n"\
        "  |                                         ||----------------------------------------- \n"\
        "  |                                         ||             .:~!!7777777!~^.             \n"\
        "  |                                         ||          .^!777!!!!!~~^^~777!^.          \n"\
        "  |                                         ||         ^777!~~^^::^~!77~::~777^         \n"\
        "  |                                         ||       .!77!~^^~~!!!~. .~77!..!777.       \n"\
        "  |               BIENVENIDO!               ||      .77~.       .^77!: .!77: ~777.      \n"\
        "  |        (el delay es a proposito)        ||      !7:            ~77^  !77. 777!      \n"\
        "  |                                         ||      7~        ....  !77  :77^ ~777      \n"\
        "  |                                         ||      777~      ~777.  ..   ..   .!7      \n"\
        "  |                                         ||      !777.     .777~            :7!      \n"\
        "  | ####################################### ||      .777!      :7777^.       .~77.      \n"\
        "  |                                         ||       .!77!:      :!7777!~~~!7777.       \n"\
        "  |  Toque cualquier tecla para continuar   ||         ^777!^.      .:^^^^~!77^         \n"\
        "  |                                         ||          .^!7777~^::::::^~!7!^.          \n"\
        "  |                                         ||             .:~!77777777!~^.             "


#define FOOTER "  |                                         ||                                          \n"\
               "   ------------------------------------------\n"



void printAnimacion(char* texto);
char* crearRegistro(char* buffer, char* palabra, unsigned apariciones);


#endif // MENU_H_INCLUDED
