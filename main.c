// PARA COMPILAR LINUX: gcc *.c -lGL -lGLU -lglut -lm -o program
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
#include "camera.h"
#include "light.h"
#include "material.h"

/** GLOBAL VARIABLES **/
GLdouble _window_ratio;              /*Control of window's proportions */
GLdouble _ortho_x_min, _ortho_x_max; /*Variables for the control of the orthographic projection*/
GLdouble _ortho_y_min, _ortho_y_max; /*Variables for the control of the orthographic projection*/
GLdouble _ortho_z_min, _ortho_z_max; /*Variables for the control of the orthographic projection*/

object3d *_first_object = 0;    /*List of objects*/
object3d *_selected_object = 0; /*Object currently selected*/
list_camera *_camera_list_first = 0;
list_camera *_selected_camera = 0;
GLint flat_smooth = 0;


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
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

/** MAIN FUNCTION **/
int main(int argc, char **argv)
{
    /*First of all, print the help information*/
    print_help();

    /* glut initializations */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(KG_WINDOW_WIDTH, KG_WINDOW_HEIGHT);
    glutInitWindowPosition(KG_WINDOW_X, KG_WINDOW_Y);
    glutCreateWindow(KG_WINDOW_TITLE);
    
    /* set the callback functions */
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // queremos poligonos opacos
    
    set_global_materials();
    set_default_lights();

    glEnable(GL_DEPTH_TEST); // activar el test de profundidad (Z-buffer)
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glShadeModel(GL_SMOOTH);  // hacen falta los vectores normales de cada vertice

    /* this initialization has to be AFTER the creation of the window */
    initialization();

    /* inizialite the transformations values */
    init_transf_values();

    /* sets default camera using add_camera() */
    set_default_cameras();

    /* start the main loop */
    glutMainLoop();
    return 0;
}
