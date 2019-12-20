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
#include "camera.h"
#include "util.h"
#include "light.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // !M_PI

#define MODO_OBJ 0
#define MODO_CAMARA 1

#define TRANSLACION 0
#define ROTACION 1
#define ESCALADO 2

#define COORD_GLOBAL 0
#define COORD_LOCAL 1

extern object3d *_first_object;
extern object3d *_selected_object;
extern list_camera *_camera_list_first;
extern list_camera *_selected_camera;

extern int modo_activo;
extern int transformacion_activa;
extern int coordenada_activa;
extern int camera_modo_obj;

transf_values *obj_up_transf_values;
transf_values *obj_down_transf_values;
transf_values *obj_right_transf_values;
transf_values *obj_left_transf_values;
transf_values *obj_avpag_transf_values;
transf_values *obj_repag_transf_values;
transf_values *obj_plus_transf_values;
transf_values *obj_minus_transf_values;

extern luz global_lights[];
extern GLint _selected_light;

/* Initializes the transformations values */
void init_transf_values()
{
    obj_up_transf_values = (transf_values *)malloc(sizeof(transf_values));
    obj_down_transf_values = (transf_values *)malloc(sizeof(transf_values));
    obj_right_transf_values = (transf_values *)malloc(sizeof(transf_values));
    obj_left_transf_values = (transf_values *)malloc(sizeof(transf_values));
    obj_avpag_transf_values = (transf_values *)malloc(sizeof(transf_values));
    obj_repag_transf_values = (transf_values *)malloc(sizeof(transf_values));
    obj_plus_transf_values = (transf_values *)malloc(sizeof(transf_values));
    obj_minus_transf_values = (transf_values *)malloc(sizeof(transf_values));

    obj_up_transf_values->scale_v = (vector3){.x = 1.0f, .y = 2.0f, .z = 1.0f};
    obj_up_transf_values->translate_v = (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f};
    obj_up_transf_values->rotation_v = (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f};

    obj_down_transf_values->scale_v = (vector3){.x = 1.0f, .y = 0.5f, .z = 1.0f};
    obj_down_transf_values->translate_v = (vector3){.x = 0.0f, .y = -1.0f, .z = 0.0f};
    obj_down_transf_values->rotation_v = (vector3){.x = -1.0f, .y = 0.0f, .z = 0.0f};

    obj_right_transf_values->scale_v = (vector3){.x = 2.0f, .y = 1.0f, .z = 1.0f};
    obj_right_transf_values->translate_v = (vector3){.x = 1.0f, .y = 0.0f, .z = 0.0f};
    obj_right_transf_values->rotation_v = (vector3){.x = 0.0f, .y = 1.0f, .z = 0.0f};

    obj_left_transf_values->scale_v = (vector3){.x = 0.5f, .y = 1.0f, .z = 1.0f};
    obj_left_transf_values->translate_v = (vector3){.x = -1.0f, .y = 0.0f, .z = 0.0f};
    obj_left_transf_values->rotation_v = (vector3){.x = 0.0f, .y = -1.0f, .z = 0.0f};

    obj_avpag_transf_values->scale_v = (vector3){.x = 1.0f, .y = 1.0f, .z = 2.0f};
    obj_avpag_transf_values->translate_v = (vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f};
    obj_avpag_transf_values->rotation_v = (vector3){.x = 0.0f, .y = 0.0f, .z = 1.0f};

    obj_repag_transf_values->scale_v = (vector3){.x = 1.0f, .y = 1.0f, .z = 0.5f};
    obj_repag_transf_values->translate_v = (vector3){.x = 0.0f, .y = 0.0f, .z = -1.0f};
    obj_repag_transf_values->rotation_v = (vector3){.x = 0.0f, .y = 0.0f, .z = -1.0f};

    obj_plus_transf_values->scale_v = (vector3){.x = 2.0f, .y = 2.0f, .z = 2.0f};

    obj_minus_transf_values->scale_v = (vector3){.x = 0.5f, .y = 0.5f, .z = 0.5f};
}

void transf_matrix_init()
{
    glMatrixMode(GL_MODELVIEW);
    if (modo_activo == MODO_OBJ)
    {
        if (coordenada_activa == COORD_LOCAL)
        {
            glLoadMatrixf(_selected_object->list_matrix->m);
        }
        else
        {
            glLoadIdentity();
        }
    }
    else if (modo_activo == MODO_CAMARA)
    {
        // modo camara
        if (coordenada_activa == COORD_LOCAL)
        {
            glLoadMatrixf(_selected_camera->actual_camera->m_inv);
        }
        else
        {
            // modo analisis
            glLoadIdentity();
            glTranslatef(-_selected_object->list_matrix->m[12], -_selected_object->list_matrix->m[13], -_selected_object->list_matrix->m[14]);
        }
    }
    else
    {
        glLoadMatrixf(global_lights[_selected_light].m_obj);
    }
}

void transf_matrix_set()
{
    GLfloat m_aux[16];
    if (modo_activo == MODO_OBJ)
    {
        list_matrix *n_elem_ptr = (list_matrix *)malloc(sizeof(list_matrix));
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

        if (camera_modo_obj == 1)
        {
            add_camera_mode_obj(_selected_object);
        }

        set_m_spotlight();
    }
    else if (modo_activo == MODO_CAMARA)
    {
        if (coordenada_activa == COORD_LOCAL)
        {
            glGetFloatv(GL_MODELVIEW_MATRIX, _selected_camera->actual_camera->m_inv);
            set_inv_m(_selected_camera);
        }
        else
        {
            glTranslatef(_selected_object->list_matrix->m[12], _selected_object->list_matrix->m[13], _selected_object->list_matrix->m[14]);
            glMultMatrixf(_selected_camera->actual_camera->m_inv);
            glGetFloatv(GL_MODELVIEW_MATRIX, _selected_camera->actual_camera->m_inv);
            set_inv_m(_selected_camera);
        }
    }
    else
    {
        glGetFloatv(GL_MODELVIEW_MATRIX, global_lights[_selected_light].m_obj);
    }
}

void transform(transf_values *values)
{
    transf_matrix_init();

    switch (transformacion_activa)
    {
    case ESCALADO:
        if (modo_activo == MODO_OBJ)
        {
            printf("Escalando\n");
            glScalef(values->scale_v.x, values->scale_v.y, values->scale_v.z);
        }

        break;
    case ROTACION:
        printf("Rotando\n");
        glRotatef(10.0f, values->rotation_v.x, values->rotation_v.y, values->rotation_v.z);
        break;
    case TRANSLACION:
        printf("Translando\n");
        glTranslatef(values->translate_v.x, values->translate_v.y, values->translate_v.z);
        break;
    }

    transf_matrix_set();
}

void analysis_mode(int x, int y)
{
    GLfloat pX, pY, pZ;
    GLfloat distance;
    GLfloat m_minus_at[16], m_plus_at[16], m_rot[16];

    pX = _selected_object->list_matrix->m[12] - _selected_camera->actual_camera->m_inv[12];
    pY = _selected_object->list_matrix->m[13] - _selected_camera->actual_camera->m_inv[13];
    pZ = _selected_object->list_matrix->m[14] - _selected_camera->actual_camera->m_inv[14];

    distance = sqrt(pow(pX, 2) + pow(pY, 2) + pow(pZ, 2));

    set_identity(m_minus_at);
    set_identity(m_plus_at);
    set_identity(m_rot);

    m_minus_at[12] = 0;
    m_minus_at[13] = 0;
    m_minus_at[14] = -distance;
    m_plus_at[12] = 0;
    m_plus_at[13] = 0;
    m_plus_at[14] = distance;

    if (x != 0)
    {
        m_rot[5] = cos(x * KG_STEP_ROTATE * M_PI / 180.0);
        m_rot[6] = sin(x * KG_STEP_ROTATE * M_PI / 180.0);
        m_rot[9] = -1 * (sin(x * KG_STEP_ROTATE * M_PI / 180.0));
        m_rot[10] = cos(x * KG_STEP_ROTATE * M_PI / 180.0);
    }
    else
    {
        m_rot[0] = cos(y * KG_STEP_ROTATE * M_PI / 180.0);
        m_rot[2] = -1 * (sin(y * KG_STEP_ROTATE * M_PI / 180.0));
        m_rot[8] = sin(y * KG_STEP_ROTATE * M_PI / 180.0);
        m_rot[10] = cos(y * KG_STEP_ROTATE * M_PI / 180.0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMultMatrixf(m_minus_at);
    glMultMatrixf(m_rot);
    glMultMatrixf(m_plus_at);
    glGetFloatv(GL_MODELVIEW_MATRIX, m_rot);

    glLoadIdentity();
    glMultMatrixf(_selected_camera->actual_camera->m_inv);
    glMultMatrixf(m_rot);
    glGetFloatv(GL_MODELVIEW_MATRIX, _selected_camera->actual_camera->m_inv);

    set_inv_m(_selected_camera);
}

GLfloat distance_camera_to_obj()
{
    GLfloat pX, pY, pZ;

    pX = _selected_object->list_matrix->m[12] - _selected_camera->actual_camera->m_inv[12];
    pY = _selected_object->list_matrix->m[13] - _selected_camera->actual_camera->m_inv[13];
    pZ = _selected_object->list_matrix->m[14] - _selected_camera->actual_camera->m_inv[14];

    return sqrt(pow(pX, 2) + pow(pY, 2) + pow(pZ, 2));
}