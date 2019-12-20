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
extern GLint flat_smooth;

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
    GLint i;

    /* Clear the screen */
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

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

    /* Now we start drawing the object */
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glLoadMatrixf(_selected_camera->actual_camera->m);

    // AQUI PUSH, mult y luego pop
    for (i = 0; i < 8; i++) 
    {
        if (global_lights[i].position != 0 && global_lights[i].is_on == 1) // luz seteada y activa
        {
            glPushMatrix();
            glMultMatrixf(global_lights[i].m_obj);
            put_light(i);
            glPopMatrix();
        }
    }

    /*Now each of the objects in the list*/
    while (aux_obj != 0)
    {
        /* Select the color, depending on whether the current object is the selected one or not */
        if (aux_obj == _selected_object)
        {
            glColor3f(KG_COL_SELECTED_R, KG_COL_SELECTED_G, KG_COL_SELECTED_B);
            glMaterialfv(GL_FRONT, GL_AMBIENT, aux_obj->material->Ka);
            glMaterialfv(GL_FRONT, GL_SPECULAR, aux_obj->material->Ks);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, aux_obj->material->Kd);
            glMaterialfv(GL_FRONT, GL_SHININESS, aux_obj->material->ns);
        }
        else
        {
            glColor3f(KG_COL_NONSELECTED_R, KG_COL_NONSELECTED_G, KG_COL_NONSELECTED_B);
            glMaterialfv(GL_FRONT, GL_AMBIENT, aux_obj->material->Ka);
            glMaterialfv(GL_FRONT, GL_SPECULAR, aux_obj->material->Ks);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, aux_obj->material->Kd);
            glMaterialfv(GL_FRONT, GL_SHININESS, aux_obj->material->ns);
        }

        /* Draw the object; for each face create a new polygon with the corresponding vertices */
        glPushMatrix();
        glMultMatrixf(aux_obj->list_matrix->m);
        for (f = 0; f < aux_obj->num_faces; f++)
        {
            glBegin(GL_POLYGON);
            
            if (aux_obj->flat_smooth == 0)
            {
                glNormal3d(
                    aux_obj->face_table[f].normal_vector.x,
                    aux_obj->face_table[f].normal_vector.y,
                    aux_obj->face_table[f].normal_vector.z
                );
            }

            for (v = 0; v < aux_obj->face_table[f].num_vertices; v++)
            {
                v_index = _selected_object->face_table[f].vertex_table[v];
                if (aux_obj->flat_smooth == 1)
                {
                    glNormal3d(
                        aux_obj->vertex_table[v_index].normal_vector.x,
                        aux_obj->vertex_table[v_index].normal_vector.y,
                        aux_obj->vertex_table[v_index].normal_vector.z
                    );
                }

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
    glutSwapBuffers();
}
