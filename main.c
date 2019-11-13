// PARA COMPILAR LINUX: gcc dibujar-puntos.c -lGL -lGLU -lglut -o program
// PARA COMPILAR EN MAC: gcc *.c -framework OpenGL -framework GLUT -lm -o program
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION true
#endif

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>

#include "display.h"
#include "io.h"
#include "definitions.h"
#include <stdlib.h>
#include "transformations.h"

/** GLOBAL VARIABLES **/

GLdouble _window_ratio;              /*Control of window's proportions */
GLdouble _ortho_x_min, _ortho_x_max; /*Variables for the control of the orthographic projection*/
GLdouble _ortho_y_min, _ortho_y_max; /*Variables for the control of the orthographic projection*/
GLdouble _ortho_z_min, _ortho_z_max; /*Variables for the control of the orthographic projection*/

object3d *_first_object = 0;    /*List of objects*/
object3d *_selected_object = 0; /*Object currently selected*/
list_camera *_camera_list_first = 0;
list_camera *_selected_camera = 0;

/** GENERAL INITIALIZATION **/
void initialization()
{
    /*Initialization of all the variables with the default values*/
    _ortho_x_min = KG_ORTHO_X_MIN_INIT;
    _ortho_x_max = KG_ORTHO_X_MAX_INIT;
    _ortho_y_min = KG_ORTHO_Y_MIN_INIT;
    _ortho_y_max = KG_ORTHO_Y_MAX_INIT;
    _ortho_z_min = KG_ORTHO_Z_MIN_INIT;
    _ortho_z_max = KG_ORTHO_Z_MAX_INIT;

    _window_ratio = (GLdouble)KG_WINDOW_WIDTH / (GLdouble)KG_WINDOW_HEIGHT;

    /*Definition of the background color*/
    glClearColor(KG_COL_BACK_R, KG_COL_BACK_G, KG_COL_BACK_B, KG_COL_BACK_A);

    /*Definition of the method to draw the objects*/
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void set_default_camera()
{
    // TODO: cambiar donde se carga la defecto, preguntar a Joseba
    /* De momento, si no hay camara, cargamos la de por defecto */
    /* Cargamos camara por defecto (primero reservar memoria) */
    camera *cm = (camera *)malloc(sizeof(camera));
    GLfloat *inv = (GLfloat *)malloc(sizeof(GLfloat) * 16);

    /* Position */
    cm->camera_pos.x = 2.0f;
    cm->camera_pos.y = 1.0f;
    cm->camera_pos.z = 4.0f;

    /* Direction */
    cm->camera_front.x = 0.0f;
    cm->camera_front.y = 0.0f;
    cm->camera_front.z = -1.0f;

    /* Vertical vector */
    cm->camera_up.x = 0.0f;
    cm->camera_up.y = 1.0f;
    cm->camera_up.z = 0.0f;

    /* Pointer to first camera of the camer lista */
    _camera_list_first = (list_camera *)malloc(sizeof(list_camera));

    _camera_list_first->actual_camera = cm;

    /* Set next camera */
    _camera_list_first->nextptr = 0;

    _selected_camera = _camera_list_first;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        _selected_camera->actual_camera->camera_pos.x,
        _selected_camera->actual_camera->camera_pos.y,
        _selected_camera->actual_camera->camera_pos.z,
        _selected_camera->actual_camera->camera_front.x,
        _selected_camera->actual_camera->camera_front.y,
        _selected_camera->actual_camera->camera_front.z,
        _selected_camera->actual_camera->camera_up.x,
        _selected_camera->actual_camera->camera_up.y,
        _selected_camera->actual_camera->camera_up.z);

    glGetFloatv(GL_MODELVIEW_MATRIX, _selected_camera->actual_camera->m);
    // no calcular inversa, mover
    // para transformacion, con la inversa

    /*
    xc.x xc.y xc.z -e.xc
    yc.x yc.y yc.z -e.yc
    zc.x zc.y zc.z -e.zc
    0    0    0   1
    */
    _selected_camera->actual_camera->m_inv[0] = _selected_camera->actual_camera->m[0];
    _selected_camera->actual_camera->m_inv[1] = _selected_camera->actual_camera->m[4];
    _selected_camera->actual_camera->m_inv[2] = _selected_camera->actual_camera->m[8];
    _selected_camera->actual_camera->m_inv[3] = _selected_camera->actual_camera->camera_pos.x;
    _selected_camera->actual_camera->m_inv[4] = _selected_camera->actual_camera->m[1];
    _selected_camera->actual_camera->m_inv[5] = _selected_camera->actual_camera->m[5];
    _selected_camera->actual_camera->m_inv[6] = _selected_camera->actual_camera->m[9];
    _selected_camera->actual_camera->m_inv[7] = _selected_camera->actual_camera->camera_pos.y;
    _selected_camera->actual_camera->m_inv[8] = _selected_camera->actual_camera->m[2];
    _selected_camera->actual_camera->m_inv[9] = _selected_camera->actual_camera->m[6];
    _selected_camera->actual_camera->m_inv[10] = _selected_camera->actual_camera->m[10];
    _selected_camera->actual_camera->m_inv[11] = _selected_camera->actual_camera->camera_pos.z;
    _selected_camera->actual_camera->m_inv[12] = 0;
    _selected_camera->actual_camera->m_inv[13] = 0;
    _selected_camera->actual_camera->m_inv[14] = 0;
    _selected_camera->actual_camera->m_inv[15] = 1;
}
/** MAIN FUNCTION **/
int main(int argc, char **argv)
{
    /*First of all, print the help information*/
    //set_default_camera();
    print_help();

    /* glut initializations */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(KG_WINDOW_WIDTH, KG_WINDOW_HEIGHT);
    glutInitWindowPosition(KG_WINDOW_X, KG_WINDOW_Y);
    glutCreateWindow(KG_WINDOW_TITLE);

    /* set the callback functions */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);

    /* this initialization has to be AFTER the creation of the window */
    initialization();

    /* inizialite the transformations values */
    init_transf_values();

    /* sets default camera using create_camera() */
    set_default_camera();

    /* start the main loop */
    glutMainLoop();
    return 0;
}
