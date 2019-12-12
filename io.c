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
#include "transformations.h"
#include "util.h"
#include "camera.h"
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

#define MODO_OBJ 0
#define MODO_CAMARA 1

#define TRASLACION 0
#define ROTACION 1
#define ESCALADO 2

#define COORD_GLOBAL 0
#define COORD_LOCAL 1

extern object3d *_first_object;
extern object3d *_selected_object;
extern list_camera *_camera_list_first;
extern list_camera *_selected_camera;
extern GLint flat_smooth;
extern GLdouble _ortho_x_min, _ortho_x_max;
extern GLdouble _ortho_y_min, _ortho_y_max;
extern GLdouble _ortho_z_min, _ortho_z_max;

extern projection *global_perspective, *global_ortho;

int modo_activo = MODO_OBJ;
int transformacion_activa = TRASLACION;
int coordenada_activa = COORD_GLOBAL;
int camera_modo_obj = 0; // 0 desactivada, 1 activada
int light_activated = 0;

vector3 camera_pos;
vector3 camera_front;
vector3 camera_up;

extern transf_values *obj_up_transf_values;
extern transf_values *obj_down_transf_values;
extern transf_values *obj_right_transf_values;
extern transf_values *obj_left_transf_values;
extern transf_values *obj_avpag_transf_values;
extern transf_values *obj_repag_transf_values;
extern transf_values *obj_plus_transf_values;
extern transf_values *obj_minus_transf_values;

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
    printf("<c> \t Cambiar de camara\n");
    printf("<C> \t Visualizar lo que ve el objeto\n");
    printf("<K> \t Activar/desactivar modo camara\n");
    printf("-Opciones con modo camara activado: \n");
    printf("<G, g> \t Camara en modo analisis\n");
    printf("<L, l> \t Camara en modo vuelo\n");
    printf("<T, t> \t Cambio de volumen de vision\n");
    printf("<B, b> \t Rotaciones a la camara\n");
    printf("<M, m> \t Traslaciones a la camara\n");
    printf("<P, p> \t Cambio de tipo de proyeccion (perpectiva/paralela)\n");

    printf("\n\n");

    printf("--- INFO ---\n");

    if (modo_activo == MODO_OBJ)
        printf(" + Activated mode: OBJECT_MODE\n");
    else
        printf(" + Activated mode: CAMERA_MODE\n");

    if (coordenada_activa == COORD_LOCAL)
        printf(" + Coordinates activated system: LOCAL\n");
    else
        printf(" + Coordinates activated system: GLOBAL\n");

    if (transformacion_activa == TRASLACION)
    {
        printf(" + Activated transformation: TRASLATION\n");
    }
    else if (transformacion_activa == ROTACION)
    {
        printf(" + Activated transformation: ROTATION\n");
    }
    else
    {
        printf(" + Activated transformation: SCALE\n");
    }
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
    camera *aux_camera = 0;
    list_camera *aux_camera_obj = 0;
    

    switch (key)
    {
    case 'f':
    case 'F':
        if (modo_activo == MODO_OBJ)
        {
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

                /* Obtenemos la matrix de indentidad del model-view */
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
        }
        else
        {
            add_camera_from_input();
        }
        break;
    case 9: /* <TAB> */
        if (_selected_object != 0)
            _selected_object = _selected_object->next;
        /*The selection is circular, thus if we move out of the list we go back to the first element*/
        if (_selected_object == 0)
            _selected_object = _first_object;

        if (modo_activo == MODO_CAMARA && coordenada_activa == COORD_GLOBAL)
            centre_camera_to_obj(_selected_object);
        if (camera_modo_obj == 1)
            add_camera_mode_obj(_selected_object);
        break;
    case 127: /* <SUPR> */
        if (_selected_object == 0)
        {
            printf("Warning: No hay objetos para borrar!\n");
        }
        else
        {
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
        }
        break;
    case CTRL_MINUS:
        if (modo_activo == MODO_CAMARA)
        {
            /*Increase the projection plane; compute the new dimensions*/
            wd = (_selected_camera->actual_camera->proj->right - _selected_camera->actual_camera->proj->left) / KG_STEP_ZOOM;
            he = (_selected_camera->actual_camera->proj->bottom - _selected_camera->actual_camera->proj->top) / KG_STEP_ZOOM;

            /*In order to avoid moving the center of the plane, we get its coordinates*/
            midx = (_selected_camera->actual_camera->proj->right + _selected_camera->actual_camera->proj->left) / 2;
            midy = (_selected_camera->actual_camera->proj->bottom + _selected_camera->actual_camera->proj->top) / 2;

            /*The the new limits are set, keeping the center of the plane*/
            _selected_camera->actual_camera->proj->right = midx + wd / 2;
            _selected_camera->actual_camera->proj->left = midx - wd / 2;
            _selected_camera->actual_camera->proj->bottom = midy + he / 2;
            _selected_camera->actual_camera->proj->top = midy - he / 2;
        }
        else
        {
            if (_selected_object != 0 && transformacion_activa == ESCALADO)
            {
                transform(obj_minus_transf_values);
            }
        }
        break;
    case CTRL_PLUS:
        if (modo_activo == MODO_CAMARA)
        {
            /*Decrease the projection plane; compute the new dimensions*/
            wd = (_selected_camera->actual_camera->proj->right - _selected_camera->actual_camera->proj->left) * KG_STEP_ZOOM;
            he = (_selected_camera->actual_camera->proj->bottom - _selected_camera->actual_camera->proj->top) * KG_STEP_ZOOM;

            /*In order to avoid moving the center of the plane, we get its coordinates*/
            midx = (_selected_camera->actual_camera->proj->right + _selected_camera->actual_camera->proj->left) / 2;
            midy = (_selected_camera->actual_camera->proj->bottom + _selected_camera->actual_camera->proj->top) / 2;

            /*The the new limits are set, keeping the center of the plane*/
            _selected_camera->actual_camera->proj->right = midx + wd / 2;
            _selected_camera->actual_camera->proj->left = midx - wd / 2;
            _selected_camera->actual_camera->proj->bottom = midy + he / 2;
            _selected_camera->actual_camera->proj->top = midy - he / 2;
        }
        else
        {
            if (_selected_object != 0 && transformacion_activa == ESCALADO)
            {
                transform(obj_plus_transf_values);
            }
        }
        break;
    case '?':
        print_help();
        break;
    /* Tipo de transformacion */
    case 'm':
    case 'M':
        if (transformacion_activa != TRASLACION)
        {
            transformacion_activa = TRASLACION;
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
            if (modo_activo == MODO_CAMARA)
                centre_camera_to_obj(_selected_object);
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
        if (modo_activo != MODO_OBJ)
        {
            modo_activo = MODO_OBJ;
            printf("Modo objeto activado!\n");
        }
        break;
    case 'a':
    case 'A':
        /* Transformaciones a la LUZ */
        break;
    case 26:
        if (_selected_object->list_matrix->nextptr != 0)
        {
            printf("CTRL-Z aplicado.\n");
            _selected_object->list_matrix = _selected_object->list_matrix->nextptr;

            if (camera_modo_obj == 1)
                add_camera_mode_obj(_selected_object);
        }
        break;
    case 27: /* <ESC> */
        exit(0);
        break;
    case 'c': // cambiar de camara
        if (camera_modo_obj == 0)
            change_camera();
        break;
    case 'C': // visualizar lo que ve el obj seleccionado (camara objeto)
        if (camera_modo_obj == 0)
        {
            camera_modo_obj = 1;
            modo_activo = MODO_OBJ;
            printf("Visualizando desde objeto (modo objeto activado)\n");
            add_camera_mode_obj(_selected_object);
        }
        else
        {
            camera_modo_obj = 0;
            printf("Saliendo de camara modo objeto...\n");
            set_default_cameras();
        }
        break;
    case 'k':
    case 'K': // activar/descativar modo camara
        if (camera_modo_obj == 0 && modo_activo != MODO_CAMARA)
        {
            if (coordenada_activa == COORD_GLOBAL)
                centre_camera_to_obj(_selected_object);
            printf("Modo camara activado!\n");
            modo_activo = MODO_CAMARA;
            break;
        }
        break;
    /* Cambiar tipo proyeccion */
    case 'p':
    case 'P':
        if (modo_activo == MODO_CAMARA)
        {
            if (_selected_camera->actual_camera->proj->type == PROJECTION_PERSPECTIVA)
            {
                printf("Perspectiva -> Ortografica\n");
                _selected_camera->actual_camera->proj = global_ortho;
            }
            else
            {
                printf("Ortografica -> Perspectiva\n");
                _selected_camera->actual_camera->proj = global_perspective;
            }
        }
        break;
    default:
        /*In the default case we just print the code of the key. This is usefull to define new cases*/
        printf("%d %c\n", key, key);
        break;
    }
    /*In case we have do any modification affecting the displaying of the object, we redraw them*/
    glutPostRedisplay();
}

void specialKeyboard(int key, int x, int y)
{
    transf_values *t_cam;
    if (_selected_object != 0)
    {
        switch (key)
        {
        case GLUT_KEY_UP:
            if (modo_activo == MODO_CAMARA)
            {
                switch (transformacion_activa)
                {
                case ESCALADO:
                    _selected_camera->actual_camera->proj->top -= 0.01;
                    _selected_camera->actual_camera->proj->bottom += 0.01;
                    break;
                case ROTACION:
                    if (coordenada_activa == COORD_GLOBAL)
                    {
                        analysis_mode(-1, 0);
                    }
                    else
                    {
                        transform(obj_up_transf_values);
                    }
                    break;
                case TRASLACION:
                    if (coordenada_activa == COORD_LOCAL)
                        transform(obj_up_transf_values);
                    break;
                default:
                    break;
                }
            }
            else
            {
                transform(obj_up_transf_values);
            }
            break;
        case GLUT_KEY_RIGHT:
            if (modo_activo == MODO_CAMARA)
            {
                switch (transformacion_activa)
                {
                case ESCALADO:
                    _selected_camera->actual_camera->proj->left -= 0.01;
                    _selected_camera->actual_camera->proj->right += 0.01;
                    break;
                case ROTACION:
                    if (coordenada_activa == COORD_GLOBAL)
                    {
                        analysis_mode(0, 1);
                    }
                    else
                    {
                        transform(obj_right_transf_values);
                    }
                    break;
                case TRASLACION:
                    if (coordenada_activa == COORD_LOCAL)
                        transform(obj_right_transf_values);
                    break;
                default:
                    break;
                }
            }
            else
            {
                transform(obj_right_transf_values);
            }
            break;
        case GLUT_KEY_LEFT:
            if (modo_activo == MODO_CAMARA)
            {
                switch (transformacion_activa)
                {
                case ESCALADO:
                    _selected_camera->actual_camera->proj->left += 0.01;
                    _selected_camera->actual_camera->proj->right -= 0.01;
                    break;
                case ROTACION:
                    if (coordenada_activa == COORD_GLOBAL)
                    {
                        analysis_mode(0, -1);
                    }
                    else
                    {
                        transform(obj_left_transf_values);
                    }
                    break;
                case TRASLACION:
                    if (coordenada_activa == COORD_LOCAL)
                        transform(obj_left_transf_values);
                    break;
                default:
                    break;
                }
            }
            else
            {
                transform(obj_left_transf_values);
            }
            break;
        case GLUT_KEY_DOWN:
            if (modo_activo == MODO_CAMARA)
            {
                switch (transformacion_activa)
                {
                case ESCALADO:
                    _selected_camera->actual_camera->proj->top += 0.01;
                    _selected_camera->actual_camera->proj->bottom -= 0.01;
                    break;
                case ROTACION:
                    if (coordenada_activa == COORD_GLOBAL)
                    {
                        analysis_mode(1, 0);
                    }
                    else
                    {
                        transform(obj_down_transf_values);
                    }
                    break;
                case TRASLACION:
                    if (coordenada_activa == COORD_LOCAL)
                        transform(obj_down_transf_values);
                    break;
                default:
                    break;
                }
            }
            else
            {
                transform(obj_down_transf_values);
            }
            break;
        case GLUT_KEY_PAGE_UP: //Repag
            if (modo_activo == MODO_CAMARA)
            {
                switch (transformacion_activa)
                {
                case ESCALADO:
                    _selected_camera->actual_camera->proj->near -= 0.01;
                    _selected_camera->actual_camera->proj->far -= 0.01;
                    break;
                case ROTACION:
                    if (coordenada_activa == COORD_LOCAL)
                        transform(obj_repag_transf_values);
                    break;
                case TRASLACION:
                    if (coordenada_activa == COORD_GLOBAL)
                    {
                        if (distance_camera_to_obj() > 2.5)
                        {
                            t_cam = (transf_values *)malloc(sizeof(transf_values));
                            t_cam->translate_v = (vector3){
                                .x = -_selected_camera->actual_camera->m_inv[8],
                                .y = -_selected_camera->actual_camera->m_inv[9],
                                .z = -_selected_camera->actual_camera->m_inv[10]};
                            transform(t_cam);
                        }
                    }
                    else
                    {
                        transform(obj_repag_transf_values);
                    }
                default:
                    break;
                }
            }
            else
            {
                transform(obj_repag_transf_values);
            }
            break;
        case GLUT_KEY_PAGE_DOWN: //AVPAG
            if (modo_activo == MODO_CAMARA && coordenada_activa == COORD_GLOBAL)
            {
                switch (transformacion_activa)
                {
                case ESCALADO:
                    _selected_camera->actual_camera->proj->near += 0.01;
                    _selected_camera->actual_camera->proj->far += 0.01;
                    break;
                case ROTACION:
                    if (coordenada_activa == COORD_LOCAL)
                        transform(obj_avpag_transf_values);
                    break;
                case TRASLACION:
                    if (coordenada_activa == COORD_GLOBAL)
                    {
                        if (distance_camera_to_obj() < 100)
                        {
                            t_cam = (transf_values *)malloc(sizeof(transf_values));
                            t_cam->translate_v = (vector3){
                                .x = _selected_camera->actual_camera->m_inv[8],
                                .y = _selected_camera->actual_camera->m_inv[9],
                                .z = _selected_camera->actual_camera->m_inv[10]};
                            transform(t_cam);
                        }
                    }
                    else
                    {
                        transform(obj_avpag_transf_values);
                    }
                default:
                    break;
                }
            }
            else
            {
                transform(obj_avpag_transf_values);
            }
            break;
        case GLUT_KEY_F9: // activa / desactiva luz
            if (light_activated == 0) 
            {
                light_activated = 1;
                glEnable(GL_LIGHTING);
            }
            else 
            {
                light_activated = 0;
                glDisable(GL_LIGHTING);
            }
            break;
        case GLUT_KEY_F1: // sol
            break;
        case GLUT_KEY_F2: // bombilla
            break;
        case GLUT_KEY_F3: // foco
            break;
        case GLUT_KEY_F12: // flat / smooth
            if (flat_smooth == 0)
            {
                flat_smooth = 1; // activar smooth
                glShadeModel(GL_SMOOTH);
            }
            else
            {
                flat_smooth = 0;
                glShadeModel(GL_FLAT);
            }
            break;
        default:
            printf("%d %c\n", key, key);
            break;
        }
        glutPostRedisplay();
    }
}
