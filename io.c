// PARA COMPILAR LINUX: gcc dibujar-puntos.c -lGL -lGLU -lglut -o program
// PARA COMPILAR EN MAC: gcc *.c -framework OpenGL -framework GLUT -lm -o program
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION true
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "definitions.h"
#include "load_obj.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef __APPLE__
#define CTRL_PLUS 29
#else
#define CTRL_PLUS '+'
#endif

#ifdef __APPLE__
#define CTRL_MINUS 47
#else
#define CTRL_MINUS '-'
#endif

#define TRANSLACION 0
#define ROTACION 1
#define ESCALADO 2

#define COORD_GLOBAL 0
#define COORD_LOCAL 1

extern object3d *_first_object;
extern object3d *_selected_object;

extern GLdouble _ortho_x_min, _ortho_x_max;
extern GLdouble _ortho_y_min, _ortho_y_max;
extern GLdouble _ortho_z_min, _ortho_z_max;

int transformacion_activa = TRANSLACION;
int coordenada_activa = COORD_GLOBAL;

void key_up_handler();
void key_down_handler();
void key_right_handler();
void key_left_handler();
void key_avpag_handler();
void key_repag_handler();
void key_plus_handler();
void key_minus_handler();

void transf_matrix_init()
{
    glMatrixMode(GL_MODELVIEW);
    if (coordenada_activa == COORD_LOCAL)
    {
        glLoadMatrixf(_selected_object->list_matrix->m);
    }
    else
    {
        glLoadIdentity();
    }
}

void transf_matrix_set(list_matrix *n_elem_ptr)
{
    if (coordenada_activa == COORD_LOCAL)
    {
        glGetFloatv(GL_MODELVIEW_MATRIX, n_elem_ptr->m);
    }
    else
    {
        glMultMatrixf(_selected_object->list_matrix->m);
        glGetFloatv(GL_MODELVIEW_MATRIX, n_elem_ptr->m);
    }

    n_elem_ptr->nextptr = _selected_object->list_matrix;
    _selected_object->list_matrix = n_elem_ptr;
}

void liberar_memoria_obj(object3d *objptr)
{
    int i;

    // LIBERAR tabla de vertices
    free(objptr->vertex_table);

    // LIBERAR cada una de las entradas de las caras, recorriendo una a una
    for (i = 0; i < objptr->num_faces; i++)
    {
        free(objptr->face_table[i].vertex_table);
    }

    // LIBERAR tabla de caras
    free(objptr->face_table);

    // LIBERAR objeto
    free(objptr);
}
/**
 * @brief This function just prints information about the use
 * of the keys
 */
void print_help()
{
    printf("KbG Irakasgaiaren Praktika. Programa honek 3D objektuak \n");
    printf("aldatzen eta bistaratzen ditu.  \n\n");
    printf("\n\n");
    printf("FUNTZIO NAGUSIAK \n");
    printf("<?>\t\t Laguntza hau bistaratu \n");
    printf("<ESC>\t\t Programatik irten \n");
    printf("<F>\t\t Objektua bat kargatu\n");
    printf("<TAB>\t\t Kargaturiko objektuen artean bat hautatu\n");
    printf("<DEL>\t\t Hautatutako objektua ezabatu\n");
    printf("<CTRL + ->\t Bistaratze-eremua handitu\n");
    printf("<CTRL + +>\t Bistaratze-eremua txikitu\n\n");
    printf("<CTRL + z>\t Deshacer\n\n");
    printf("- Tipo de transformacion\n");
    printf("<M, m> \t Activar Translacion\n");
    printf("<B, b> \t Activar Rotacion\n");
    printf("<T, t> \t Activar Escalado\n");
    printf("- Sistema de referencia\n");
    printf("<G, g> \t Activar transformaciones globales\n");
    printf("<T, t> \t Activar transformaciones locales\n");

    printf("\n\n");
}

/**
 * @brief Callback function to control the basic keys
 * @param key Key that has been pressed
 * @param x X coordinate of the mouse pointer when the key was pressed
 * @param y Y coordinate of the mouse pointer when the key was pressed
 */
void keyboard(unsigned char key, int x, int y)
{
    char *fname = malloc(sizeof(char) * 128); /* Note that scanf adds a null character at the end of the vector*/
    int read = 0;
    object3d *auxiliar_object = 0;
    list_matrix *aux_list;
    GLdouble wd, he, midx, midy;
    int i;

    switch (key)
    {
    case 'f':
    case 'F':
        /*Ask for file*/
        printf("%s", KG_MSSG_SELECT_FILE);
        scanf("%s", fname);
        /*Allocate memory for the structure and read the file*/
        auxiliar_object = (object3d *)malloc(sizeof(object3d));
        read = read_wavefront(fname, auxiliar_object);
        switch (read)
        {
        /*Errors in the reading*/
        case 1:
            printf("%s: %s\n", fname, KG_MSSG_FILENOTFOUND);
            break;
        case 2:
            printf("%s: %s\n", fname, KG_MSSG_INVALIDFILE);
            break;
        case 3:
            printf("%s: %s\n", fname, KG_MSSG_EMPTYFILE);
            break;
        /*Read OK*/
        case 0:
            /* Reservar memoria para list_matrix */
            aux_list = (list_matrix *)malloc(sizeof(list_matrix));

            /* Obtenemos la matrix de indentidad del mopdel-view */
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glGetFloatv(GL_MODELVIEW_MATRIX, aux_list->m);
            aux_list->nextptr = 0;
            auxiliar_object->list_matrix = aux_list;

            /*Insert the new object in the list*/
            auxiliar_object->next = _first_object;
            _first_object = auxiliar_object;
            _selected_object = _first_object;
            printf("%s\n", KG_MSSG_FILEREAD);
            break;
        }
        break;

    case 9: /* <TAB> */
        if (_selected_object != 0)
        {
            _selected_object = _selected_object->next;
        }
        /*The selection is circular, thus if we move out of the list we go back to the first element*/
        if (_selected_object == 0)
            _selected_object = _first_object;
        break;

    case 127: /* <SUPR> */
        /*Erasing an object depends on whether it is the first one or not*/
        if (_selected_object == _first_object)
        {
            /*To remove the first object we just set the first as the current's next*/
            _first_object = _first_object->next;
            /*Once updated the pointer to the first object it is save to free the memory*/

            liberar_memoria_obj(_selected_object);

            /*Finally, set the selected to the new first one*/
            _selected_object = _first_object;
        }
        else
        {
            /*In this case we need to get the previous element to the one we want to erase*/
            auxiliar_object = _first_object;
            while (auxiliar_object->next != _selected_object)
                auxiliar_object = auxiliar_object->next;
            /*Now we bypass the element to erase*/
            auxiliar_object->next = _selected_object->next;
            /*free the memory*/
            liberar_memoria_obj(_selected_object);
            /*and update the selection*/
            _selected_object = auxiliar_object;
        }
        break;

    case CTRL_MINUS:
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
        {
            /*Increase the projection plane; compute the new dimensions*/
            wd = (_ortho_x_max - _ortho_x_min) / KG_STEP_ZOOM;
            he = (_ortho_y_max - _ortho_y_min) / KG_STEP_ZOOM;
            /*In order to avoid moving the center of the plane, we get its coordinates*/
            midx = (_ortho_x_max + _ortho_x_min) / 2;
            midy = (_ortho_y_max + _ortho_y_min) / 2;
            /*The the new limits are set, keeping the center of the plane*/
            _ortho_x_max = midx + wd / 2;
            _ortho_x_min = midx - wd / 2;
            _ortho_y_max = midy + he / 2;
            _ortho_y_min = midy - he / 2;
        }
        break;

    case CTRL_PLUS:
        if (glutGetModifiers() == GLUT_ACTIVE_CTRL)
        {
            /*Decrease the projection plane; compute the new dimensions*/
            wd = (_ortho_x_max - _ortho_x_min) * KG_STEP_ZOOM;
            he = (_ortho_y_max - _ortho_y_min) * KG_STEP_ZOOM;
            /*In order to avoid moving the center of the plane, we get its coordinates*/
            midx = (_ortho_x_max + _ortho_x_min) / 2;
            midy = (_ortho_y_max + _ortho_y_min) / 2;
            /*The the new limits are set, keeping the center of the plane*/
            _ortho_x_max = midx + wd / 2;
            _ortho_x_min = midx - wd / 2;
            _ortho_y_max = midy + he / 2;
            _ortho_y_min = midy - he / 2;
        }
        break;

    case '?':
        print_help();
        break;

    /* Tipo de transformacion */
    case 'm':
    case 'M':

        if (transformacion_activa != TRANSLACION)
        {
            transformacion_activa = TRANSLACION;
            printf("Translacion activada!\n");
        }

        break;

    case 'b':
    case 'B':
        if (transformacion_activa != ROTACION)
        {
            transformacion_activa = ROTACION;
            printf("Rotacion activada!\n");
        }
        break;

    case 't':
    case 'T':
        if (transformacion_activa != ESCALADO)
        {
            transformacion_activa = ESCALADO;
            printf("ESCALADO activada!\n");
        }

        break;

    /* Sistema de referencia (modos excluyentes entre si) */
    case 'g':
    case 'G':
        if (coordenada_activa != COORD_GLOBAL)
        {
            coordenada_activa = COORD_GLOBAL;
            printf("Transformacion mundo activada!\n");
        }
        break;

    case 'l':
    case 'L':
        if (coordenada_activa != COORD_LOCAL)
        {
            coordenada_activa = COORD_LOCAL;
            printf("Transformacion local activada!\n");
        }
        break;

    /* Elemento a transformar (en todo momento se debe en algun modo. Excluyentes entre si) */
    case 'o':
    case 'O':

        break;

    case 'k':
    case 'K':

        break;

    case 'a':
    case 'A':
        break;
        /* 
    TODO: -- CAMBIAR --
    101 e -> +
    114 r -> -
    */
    case 'e':
    case 'E':
        if (_selected_object != 0)
        {
            key_plus_handler();
        }
        break;
    case 'r':
    case 'R':
        if (_selected_object != 0)
        {
            key_minus_handler();
        }
        break;
    case 26:
        if (_selected_object->list_matrix->nextptr != 0)
        {
            printf("CTRL-Z aplicado.\n");
            _selected_object->list_matrix = _selected_object->list_matrix->nextptr;
        }
        break;
    case 27: /* <ESC> */
        exit(0);
        break;

    default:
        /*In the default case we just print the code of the key. This is usefull to define new cases*/
        printf("%d %c\n", key, key);
        break;
    }
    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();
}

void key_up_handler()
{
    /*
    Trasladar +Y; Escalar + Y; Rotar +X
    */
    list_matrix *n_elem_ptr = (list_matrix *)malloc(sizeof(list_matrix));

    transf_matrix_init();

    switch (transformacion_activa)
    {
    case ESCALADO:
        printf("Escalando\n");
        glScalef(1.0f, 1.5f, 1.0f);
        break;
    case ROTACION:
        printf("Rotando\n");
        glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
        break;
    case TRANSLACION:
        printf("Translando\n");
        glTranslatef(0.0f, 1.0f, 0.0f);
        break;
    }

    transf_matrix_set(n_elem_ptr);
}

void key_down_handler()
{
    /*
    Trasladar -Y; Escalar - Y; Rotar -X
    */
    list_matrix *n_elem_ptr = malloc(sizeof(list_matrix));

    transf_matrix_init();

    switch (transformacion_activa)
    {
    case ESCALADO:
        printf("Escalando\n");
        glScalef(1.0f, 0.5f, 1.0f);
        break;
    case ROTACION:
        printf("Rotando\n");
        glRotatef(45.0f, -1.0f, 0.0f, 0.0f);
        break;
    case TRANSLACION:
        printf("Translando\n");
        glTranslatef(0.0f, -1.0f, 0.0f);
        break;
    }

    transf_matrix_set(n_elem_ptr);
}

void key_right_handler()
{
    /*
    Trasladar +X; Escalar + X; Rotar +Y
    */
    list_matrix *n_elem_ptr = malloc(sizeof(list_matrix));

    transf_matrix_init();

    switch (transformacion_activa)
    {
    case ESCALADO:
        printf("Escalando\n");
        glScalef(1.5f, 1.0f, 1.0f);
        break;
    case ROTACION:
        printf("Rotando\n");
        glRotatef(45.0f, 0.0f, 1.0f, 0.0f);
        break;
    case TRANSLACION:
        printf("Translando\n");
        glTranslatef(1.0f, 0.0f, 0.0f);
        break;
    }

    transf_matrix_set(n_elem_ptr);
}

void key_left_handler()
{
    /*
    Trasladar -X; Escalar - X; Rotar -Y
    */
    list_matrix *n_elem_ptr = malloc(sizeof(list_matrix));

    transf_matrix_init();

    switch (transformacion_activa)
    {
    case ESCALADO:
        printf("Escalando\n");
        glScalef(0.5f, 1.0f, 1.0f);
        break;
    case ROTACION:
        printf("Rotando\n");
        glRotatef(45.0f, 0.0f, -1.0f, 0.0f);
        break;
    case TRANSLACION:
        printf("Translando\n");
        glTranslatef(-1.0f, 0.0f, 0.0f);
        break;
    }

    transf_matrix_set(n_elem_ptr);
}

void key_avpag_handler()
{
    /*
    Trasladar +Z; Escalar + Z; Rotar +Z
    */
    list_matrix *n_elem_ptr = malloc(sizeof(list_matrix));

    transf_matrix_init();

    switch (transformacion_activa)
    {
    case ESCALADO:
        printf("Escalando\n");
        glScalef(1.0f, 1.0f, 1.5f);
        break;
    case ROTACION:
        printf("Rotando\n");
        glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
        break;
    case TRANSLACION:
        printf("Translando\n");
        glTranslatef(0.0f, 0.0f, 1.0f);
        break;
    }

    transf_matrix_set(n_elem_ptr);
}
/* TODO: terminar */
void key_repag_handler()
{
    /*
    Trasladar -Z; Escalar - Z; Rotar -Z
    */
    list_matrix *n_elem_ptr = malloc(sizeof(list_matrix));

    transf_matrix_init();

    switch (transformacion_activa)
    {
    case ESCALADO:
        printf("Escalando\n");
        glScalef(1.0f, 1.0f, 0.5f);
        break;
    case ROTACION:
        printf("Rotando\n");
        glRotatef(45.0f, 0.0f, 0.0f, -1.0f);
        break;
    case TRANSLACION:
        printf("Translando\n");
        glTranslatef(0.0f, 0.0f, -1.0f);
        break;
    }

    transf_matrix_set(n_elem_ptr);
}

void key_plus_handler()
{
    /*
    Escalar + en todos los ejes (solo objetos)
    */
    list_matrix *n_elem_ptr = malloc(sizeof(list_matrix));
    
    transf_matrix_init();

    if (transformacion_activa == ESCALADO)
    {
        glScalef(1.5f, 1.5f, 1.5f);
        printf("Escalando\n");
    }

    transf_matrix_set(n_elem_ptr);
}

void key_minus_handler()
{
    /*
    Escalar -  en todos los ejes (solo objetos)
    */
    list_matrix *n_elem_ptr = malloc(sizeof(list_matrix));
    
    transf_matrix_init();

    if (transformacion_activa == ESCALADO)
    {
        glScalef(0.5f, 0.5f, 0.5f);
        printf("Escalando\n");
    }

    transf_matrix_set(n_elem_ptr);
}

void specialKeyboard(int key, int x, int y)
{
    if (_selected_object != 0)
    {
        switch (key)
        {
        case GLUT_KEY_UP:
            key_up_handler();
            break;
        case GLUT_KEY_RIGHT:
            key_right_handler();
            break;
        case GLUT_KEY_LEFT:
            key_left_handler();
            break;
        case GLUT_KEY_DOWN:
            key_down_handler();
            break;
        case GLUT_KEY_PAGE_UP: //Repag
            key_repag_handler();
            break;
        case GLUT_KEY_PAGE_DOWN: //AVPAG
            key_avpag_handler();
            break;
        default:
            printf("%d %c\n", key, key);
            break;
        }
        glutPostRedisplay();
    }
}
