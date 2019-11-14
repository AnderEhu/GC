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
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "definitions.h"

/** EXTERNAL VARIABLES **/

extern GLdouble _window_ratio;
extern GLdouble _ortho_x_min, _ortho_x_max;
extern GLdouble _ortho_y_min, _ortho_y_max;
extern GLdouble _ortho_z_min, _ortho_z_max;

extern object3d *_first_object;
extern object3d *_selected_object;
extern list_camera *_selected_camera;

/**
 * @brief Function to draw the axes
 */
void draw_axes()
{
    /*Draw X axis*/
    glColor3f(KG_COL_X_AXIS_R, KG_COL_X_AXIS_G, KG_COL_X_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(KG_MAX_DOUBLE, 0, 0);
    glVertex3d(-1 * KG_MAX_DOUBLE, 0, 0);
    glEnd();
    /*Draw Y axis*/
    glColor3f(KG_COL_Y_AXIS_R, KG_COL_Y_AXIS_G, KG_COL_Y_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0, KG_MAX_DOUBLE, 0);
    glVertex3d(0, -1 * KG_MAX_DOUBLE, 0);
    glEnd();
    /*Draw Z axis*/
    glColor3f(KG_COL_Z_AXIS_R, KG_COL_Z_AXIS_G, KG_COL_Z_AXIS_B);
    glBegin(GL_LINES);
    glVertex3d(0, 0, KG_MAX_DOUBLE);
    glVertex3d(0, 0, -1 * KG_MAX_DOUBLE);
    glEnd();
}

/**
 * @brief Callback reshape function. We just store the new proportions of the window
 * @param width New width of the window
 * @param height New height of the window
 */
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    /*  Take care, the width and height are integer numbers, but the ratio is a GLdouble so, in order to avoid
     *  rounding the ratio to integer values we need to cast width and height before computing the ratio */
    _window_ratio = (GLdouble)width / (GLdouble)height;
}

/**
 * @brief Callback display function
 */
void display(void)
{
    GLint v_index, v, f;
    object3d *aux_obj = _first_object;

    /* Clear the screen */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Define the projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (_selected_camera->actual_camera->projection_type == PROJECTION_PERSPECTIVA)
    {
        gluPerspective(
            _selected_camera->actual_camera->angle, 
            _window_ratio, 
            _selected_camera->actual_camera->near,
            _selected_camera->actual_camera->far
        );
    }
    else
    {
        glOrtho(
            _selected_camera->actual_camera->left,
            _selected_camera->actual_camera->right,
            _selected_camera->actual_camera->bottom,
            _selected_camera->actual_camera->top,
            _selected_camera->actual_camera->near,
            _selected_camera->actual_camera->far
        );
    }
    
    /*First, we draw the axes*/
    draw_axes();

    /* Now we start drawing the object */
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    glLoadMatrixf(_selected_camera->actual_camera->m);
    /*Now each of the objects in the list*/
    while (aux_obj != 0)
    {
        /* Select the color, depending on whether the current object is the selected one or not */
        if (aux_obj == _selected_object)
        {
            glColor3f(KG_COL_SELECTED_R, KG_COL_SELECTED_G, KG_COL_SELECTED_B);
        }
        else
        {
            glColor3f(KG_COL_NONSELECTED_R, KG_COL_NONSELECTED_G, KG_COL_NONSELECTED_B);
        }

        /* Draw the object; for each face create a new polygon with the corresponding vertices */
        glPushMatrix();
        glMultMatrixf(aux_obj->list_matrix->m);
        for (f = 0; f < aux_obj->num_faces; f++)
        {
            glBegin(GL_POLYGON);
            for (v = 0; v < aux_obj->face_table[f].num_vertices; v++)
            {
                v_index = aux_obj->face_table[f].vertex_table[v];
                glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                           aux_obj->vertex_table[v_index].coord.y,
                           aux_obj->vertex_table[v_index].coord.z);
            }
            glEnd();
        }
        glPopMatrix();
        aux_obj = aux_obj->next;
    }
    /*Do the actual drawing*/
    glFlush();
}
