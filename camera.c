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

#include "definitions.h"
#include <stdlib.h>
#include <stdio.h>

extern list_camera *_camera_list_first;
extern list_camera *_selected_camera;

projection *global_perspective, *global_ortho;

/* Initialize the camera projections */
void init_camera_projections()
{
    global_perspective = (projection*)malloc(sizeof(projection));
    global_ortho = (projection*)malloc(sizeof(projection));

    global_perspective->type = PROJECTION_PERSPECTIVA;
    global_ortho->type = PROJECTION_ORTOGRAFICA;

    /* PERSPECTIVA */
    global_perspective->left = -0.1;
    global_perspective->right = 0.1;
    global_perspective->bottom = -0.1;
    global_perspective->top = 0.1;
    global_perspective->near = 0.1;
    global_perspective->far = 100;

    /* ORTOGRAFICA */
    global_ortho->left = -3.0;
    global_ortho->right = 3;
    global_ortho->bottom = -3.0;
    global_ortho->top = 3.0;
    global_ortho->near = 0.0;
    global_ortho->far = 100;
}

list_camera* create_camera(vector3 camera_pos, vector3 camera_front, vector3 camera_up)
{
    camera *cm = (camera*)malloc(sizeof(camera));
    GLfloat *inv = (GLfloat *)malloc(sizeof(GLfloat) * 16);
    list_camera *aux_list = (list_camera*)malloc(sizeof(list_camera));

    aux_list->nextptr = 0;

    aux_list->actual_camera = cm; // projection formated correctly at this point
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        camera_pos.x,
        camera_pos.y,
        camera_pos.z,
        camera_front.x,
        camera_front.y,
        camera_front.z,
        camera_up.x,
        camera_up.y,
        camera_up.z
    );

    glGetFloatv(GL_MODELVIEW_MATRIX, aux_list->actual_camera->m);

    /*
    xc.x xc.y xc.z -e.xc
    yc.x yc.y yc.z -e.yc
    zc.x zc.y zc.z -e.zc
    0    0    0   1
    */
    aux_list->actual_camera->m_inv[0] = aux_list->actual_camera->m[0];
    aux_list->actual_camera->m_inv[1] = aux_list->actual_camera->m[4];
    aux_list->actual_camera->m_inv[2] = aux_list->actual_camera->m[8];
    aux_list->actual_camera->m_inv[3] = 0;
    aux_list->actual_camera->m_inv[4] = aux_list->actual_camera->m[1];
    aux_list->actual_camera->m_inv[5] = aux_list->actual_camera->m[5];
    aux_list->actual_camera->m_inv[6] = aux_list->actual_camera->m[9];
    aux_list->actual_camera->m_inv[7] = 0;
    aux_list->actual_camera->m_inv[8] = aux_list->actual_camera->m[2];
    aux_list->actual_camera->m_inv[9] = aux_list->actual_camera->m[6];
    aux_list->actual_camera->m_inv[10] = aux_list->actual_camera->m[10];
    aux_list->actual_camera->m_inv[11] = 0;
    aux_list->actual_camera->m_inv[12] = camera_pos.x;
    aux_list->actual_camera->m_inv[13] = camera_pos.y;
    aux_list->actual_camera->m_inv[14] = camera_pos.z;
    aux_list->actual_camera->m_inv[15] = 1;

    return aux_list;
}

/* Adds camera to the camera list */
void add_camera_to_list(list_camera *l_camera) 
{
    _selected_camera->nextptr = l_camera;
    _selected_camera = l_camera;
}

/* Sets the default camera adding the first camera of the camera list */
void set_default_cameras()
{
    init_camera_projections();
    list_camera *aux_list = (list_camera*)malloc(sizeof(list_camera));

    vector3 cam_pos = (vector3){ .x = 4.0f, .y = 3.0f, .z = 3.0f };
    vector3 cam_front = (vector3){ .x = 0.0f, .y = 0.0f, .z = 0.0f };
    vector3 cam_vup = (vector3){ .x = 0.0f, .y = 1.0f, .z = 0.0f };
    
    aux_list = create_camera(cam_pos, cam_front, cam_vup);
    
    /* Default cameras projection is setted manually */
    aux_list->actual_camera->proj = global_perspective;

    /* Pointer to first camera of the camera list */
    _camera_list_first = (list_camera *)malloc(sizeof(list_camera));
    _camera_list_first = aux_list;

    /* Set next camera */
    _camera_list_first->nextptr = 0;

    /* Set the selected camera pointer */
    _selected_camera = _camera_list_first;

    /* Info message for default camera */
    printf(
        " + Default camera loaded at: x=%.1f y=%.1f z=%.1f, looking to x=%.1f y=%.1f z=%.1f.\n", 
        cam_pos.x, cam_pos.y, cam_pos.z, cam_front.x, cam_front.y, cam_front.z
    );
}

/* Changes camera to the next camera of the camera list (circular list) */
void change_camera()
{
    if (_selected_camera != 0)
        _selected_camera = _selected_camera->nextptr;
    if (_selected_camera == 0)
        _selected_camera = _camera_list_first;
}

/* Adds the camera inserted to the list, and selects this camera as selected_camera */
void add_camera_from_input()
{
    vector3 pos, front, vup;
    list_camera *aux_list = (list_camera*)malloc(sizeof(list_camera));

    printf(" --- Inserting camera --- \n");
    printf(" Format: three componentes separated with spaces. \n");
    printf(" Example: 1 50 60, this sets x=1, y=50, z=60.\n");
    printf(" Insert position [x y z]: \n");
    scanf("%lf %lf %lf", &pos.x, &pos.y, &pos.z);
    printf(" Insert front [x y z]: \n");
    scanf("%lf %lf %lf", &front.x, &front.y, &front.z);
    vup = (vector3) { .x = 0, .y = 1, .z = 0 };

    printf(" Inserting camera...\n");

    aux_list = create_camera(pos, front, vup);

    printf(" Adding camera to list... \n");

    add_camera_to_list(aux_list);

    aux_list->actual_camera->proj = global_perspective;

    printf(" Camera added to list.\n");
}

void set_inv_m(list_camera *c)
{
    c->actual_camera->m[0] = c->actual_camera->m_inv[0];
    c->actual_camera->m[4] = c->actual_camera->m_inv[1];
    c->actual_camera->m[8] = c->actual_camera->m_inv[2];
    c->actual_camera->m[12] = -(
        c->actual_camera->m_inv[12] * c->actual_camera->m_inv[0] +
        c->actual_camera->m_inv[13] * c->actual_camera->m_inv[1] +
        c->actual_camera->m_inv[14] * c->actual_camera->m_inv[2]
        );

    c->actual_camera->m[1] = c->actual_camera->m_inv[4];
    c->actual_camera->m[5] = c->actual_camera->m_inv[5];
    c->actual_camera->m[9] = c->actual_camera->m_inv[6];
    c->actual_camera->m[13] = -(
        c->actual_camera->m_inv[12] * c->actual_camera->m_inv[4] +
        c->actual_camera->m_inv[13] * c->actual_camera->m_inv[5] +
        c->actual_camera->m_inv[14] * c->actual_camera->m_inv[6]
        );

    c->actual_camera->m[2] = c->actual_camera->m_inv[8];
    c->actual_camera->m[6] = c->actual_camera->m_inv[9];
    c->actual_camera->m[10] = c->actual_camera->m_inv[10];
    c->actual_camera->m[14] = -(
        c->actual_camera->m_inv[12] * c->actual_camera->m_inv[8] +
        c->actual_camera->m_inv[13] * c->actual_camera->m_inv[9] +
        c->actual_camera->m_inv[14] * c->actual_camera->m_inv[10]
        );

    c->actual_camera->m[3] = 0;
    c->actual_camera->m[7] = 0;
    c->actual_camera->m[11] = 0;
    c->actual_camera->m[15] = 1;
}

void add_camera_mode_obj(object3d *obj)
{
    _selected_camera->actual_camera->m_inv[0] = obj->list_matrix->m[0];
    _selected_camera->actual_camera->m_inv[1] = obj->list_matrix->m[4];
    _selected_camera->actual_camera->m_inv[2] = -obj->list_matrix->m[8];
    _selected_camera->actual_camera->m_inv[3] = 0;
    _selected_camera->actual_camera->m_inv[4] = obj->list_matrix->m[1];
    _selected_camera->actual_camera->m_inv[5] = obj->list_matrix->m[5];
    _selected_camera->actual_camera->m_inv[6] = -obj->list_matrix->m[9];
    _selected_camera->actual_camera->m_inv[7] = 0;
    _selected_camera->actual_camera->m_inv[8] = obj->list_matrix->m[2];
    _selected_camera->actual_camera->m_inv[9] = obj->list_matrix->m[6];
    _selected_camera->actual_camera->m_inv[10] = -obj->list_matrix->m[10];
    _selected_camera->actual_camera->m_inv[11] = 0;
    _selected_camera->actual_camera->m_inv[12] = obj->list_matrix->m[12];
    _selected_camera->actual_camera->m_inv[13] = obj->list_matrix->m[13];
    _selected_camera->actual_camera->m_inv[14] = obj->list_matrix->m[14];
    _selected_camera->actual_camera->m_inv[15] = 1;

    set_inv_m(_selected_camera);
}

void exit_camera_mode_obj(object3d *obj)
{
    
}

vector3 get_camera_e()
{
    return (vector3) { .x = _selected_camera->actual_camera->m_inv[12], .y = _selected_camera->actual_camera->m_inv[13], .z =  _selected_camera->actual_camera->m_inv[14] };
}