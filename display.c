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

#include <stdio.h>
#include "definitions.h"
#include "math.h"
#include "light.h"

/** EXTERNAL VARIABLES **/

extern GLdouble _window_ratio;
extern GLdouble _ortho_x_min, _ortho_x_max;
extern GLdouble _ortho_y_min, _ortho_y_max;
extern GLdouble _ortho_z_min, _ortho_z_max;

extern object3d *_first_object;
extern object3d *_selected_object;
extern list_camera *_selected_camera;
extern luz global_lights[];

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

vector3 calc_vect_normal(vertex *vertex_table, GLint i1, GLint i2, GLint i3)
{
    vector3 U, V, v_normal;

    U.x = vertex_table[i2].coord.x - vertex_table[i1].coord.x;
    U.y = vertex_table[i2].coord.y - vertex_table[i1].coord.y;
    U.z = vertex_table[i2].coord.z - vertex_table[i1].coord.z;

    V.x = vertex_table[i3].coord.x - vertex_table[i1].coord.x;
    V.y = vertex_table[i3].coord.y - vertex_table[i1].coord.y;
    V.z = vertex_table[i3].coord.z - vertex_table[i1].coord.z;


    v_normal.x = (U.y * V.z) - (U.z * V.y);
    v_normal.y = (U.z * V.x) - (U.x * V.z);
    v_normal.z = (U.x * V.y) - (U.y * V.x);

    return v_normal;
}

/**
 * @brief Callback display function
 */
void display(void)
{
    GLint v_index, v, f;
    GLint i1, i2, i3;
    object3d *aux_obj = _first_object;
    vector3 v_normal;
    GLfloat norma;
    vector3 v_normal_init = (vector3) { .x = 0, .y = 0, .z = 0 };

    /*GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };*/
    GLfloat light_position[] = { 0.0, 1.0, 0.0, 0.0 };
    GLfloat mat_ambient[] = { 0.25f, 0.148f, 0.06475f, 1.0f  };
    GLfloat mat_diffuse[] = { 0.4f, 0.2368f, 0.1036f, 1.0f };
    GLfloat mat_specular[] = { 0.774597f, 0.458561f, 0.200621f, 1.0f };
    GLfloat mat_shine[] = { 76.8f };

    /* Clear the screen */
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    /* Define the projection */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (_selected_camera->actual_camera->proj->type == PROJECTION_PERSPECTIVA)
    {
        glFrustum(
            _selected_camera->actual_camera->proj->left,
            _selected_camera->actual_camera->proj->right,
            _selected_camera->actual_camera->proj->bottom,
            _selected_camera->actual_camera->proj->top,
            _selected_camera->actual_camera->proj->near,
            _selected_camera->actual_camera->proj->far);
    }
    else
    {
        glOrtho(
            _selected_camera->actual_camera->proj->left * _window_ratio,
            _selected_camera->actual_camera->proj->right * _window_ratio,
            _selected_camera->actual_camera->proj->bottom,
            _selected_camera->actual_camera->proj->top,
            _selected_camera->actual_camera->proj->near,
            _selected_camera->actual_camera->proj->far);
    }

    /*First, we draw the axes*/
    // draw_axes();

    /* Now we start drawing the object */
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glLoadMatrixf(_selected_camera->actual_camera->m);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shine);
    if (global_lights[0].is_on == 1) glLightfv(GL_LIGHT0, GL_POSITION, global_lights[0].position);
    if (global_lights[1].is_on == 1) glLightfv(GL_LIGHT1, GL_POSITION, global_lights[1].position);
    
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
            
            i1 = aux_obj->face_table[f].vertex_table[0];
            i2 = aux_obj->face_table[f].vertex_table[1];
            i3 = aux_obj->face_table[f].vertex_table[2];

            v_normal = calc_vect_normal(aux_obj->vertex_table, i1, i2, i3);

            //printf("%lf %lf %lf\n", v_normal.x, v_normal.y, v_normal.z);

            for (v = 0; v < aux_obj->face_table[f].num_vertices; v++)
            {
                v_index = aux_obj->face_table[f].vertex_table[v];
                aux_obj->face_table[f].normal_vector = v_normal;

                aux_obj->vertex_table[v_index].normal_vector = v_normal_init;

                //printf("%lf %lf %lf\n", aux_obj->face_table[f].normal_vector.x, aux_obj->face_table[f].normal_vector.y, aux_obj->face_table[f].normal_vector.z);
                
                aux_obj->vertex_table[v_index].normal_vector.x += aux_obj->face_table[f].normal_vector.x;
                aux_obj->vertex_table[v_index].normal_vector.y += aux_obj->face_table[f].normal_vector.y;
                aux_obj->vertex_table[v_index].normal_vector.z += aux_obj->face_table[f].normal_vector.z;

                glVertex3d(aux_obj->vertex_table[v_index].coord.x,
                           aux_obj->vertex_table[v_index].coord.y,
                           aux_obj->vertex_table[v_index].coord.z);
                
                glNormal3d(aux_obj->face_table[f].normal_vector.x,
                           aux_obj->face_table[f].normal_vector.y,
                           aux_obj->face_table[f].normal_vector.z);
            }
            glEnd();
        }

        // hacer unitario
        for (f = 0; f < aux_obj->num_faces; f++)
        {
            for (v = 0; v < aux_obj->face_table[f].num_vertices; v++)
            {
                v_index = aux_obj->face_table[f].vertex_table[v];
                
                norma = sqrt(
                    pow(aux_obj->vertex_table[v_index].normal_vector.x, 2) + 
                    pow(aux_obj->vertex_table[v_index].normal_vector.y, 2) + 
                    pow(aux_obj->vertex_table[v_index].normal_vector.z, 2)
                );

                aux_obj->vertex_table[v_index].normal_vector.x /= norma;
                aux_obj->vertex_table[v_index].normal_vector.y /= norma;
                aux_obj->vertex_table[v_index].normal_vector.z /= norma;

                //printf("%f %f %f\n", aux_obj->vertex_table[v_index].normal_vector.x, aux_obj->vertex_table[v_index].normal_vector.y, aux_obj->vertex_table[v_index].normal_vector.z);
            }
        }
        
        glPopMatrix();
        aux_obj = aux_obj->next;
    }
    /*Do the actual drawing*/
    glFlush();
    glutSwapBuffers();
}
