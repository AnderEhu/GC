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

/* Sets the camera projection type */
void set_camera_projection(camera *c)
{
    // TODO: errors checkin
    printf(" Inserting projection...\n");
    printf(" Insert projection type: \n");
    printf(" 1 - Perspective or 2 - Ortografic \n");
    scanf("%d", &c->projection_type);
    if (c->projection_type == PROJECTION_PERSPECTIVA)
    {
        printf(" Insert vision angle: \n");
        scanf("%f", &c->angle);
    }
    else
    {
        printf(" Insert left point: \n");
        scanf("%f", &c->left);
        printf(" Insert top point: \n");
        scanf("%f", &c->top);
    }
    printf(" Insert right point: \n");
    scanf("%f", &c->right);
    printf(" Insert bottom point: \n");
    scanf("%f", &c->bottom);
    printf(" Insert near point (must be > 0): \n");
    scanf("%f", &c->near);
    printf(" Insert far point (must be > near): \n");
    scanf("%f", &c->far);
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
   // reorganize for the m inverse
   // TODO: check
    aux_list->actual_camera->m_inv[0] = aux_list->actual_camera->m[0];
    aux_list->actual_camera->m_inv[1] = aux_list->actual_camera->m[4];
    aux_list->actual_camera->m_inv[2] = aux_list->actual_camera->m[8];
    aux_list->actual_camera->m_inv[3] = camera_pos.x;
    aux_list->actual_camera->m_inv[4] = aux_list->actual_camera->m[1];
    aux_list->actual_camera->m_inv[5] = aux_list->actual_camera->m[5];
    aux_list->actual_camera->m_inv[6] = aux_list->actual_camera->m[9];
    aux_list->actual_camera->m_inv[7] = camera_pos.y;
    aux_list->actual_camera->m_inv[8] = aux_list->actual_camera->m[2];
    aux_list->actual_camera->m_inv[9] = aux_list->actual_camera->m[6];
    aux_list->actual_camera->m_inv[10] = aux_list->actual_camera->m[10];
    aux_list->actual_camera->m_inv[11] = camera_pos.z;
    aux_list->actual_camera->m_inv[12] = 0;
    aux_list->actual_camera->m_inv[13] = 0;
    aux_list->actual_camera->m_inv[14] = 0;
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
    list_camera *aux_list = (list_camera*)malloc(sizeof(list_camera));

    vector3 cam_pos = (vector3){ .x = 4.0f, .y = 3.0f, .z = 3.0f };
    vector3 cam_front = (vector3){ .x = 0.0f, .y = 0.0f, .z = 0.0f };
    vector3 cam_vup = (vector3){ .x = 0.0f, .y = 1.0f, .z = 0.0f };
    
    aux_list = create_camera(cam_pos, cam_front, cam_vup);
    
    /* Default cameras projection is setted manually */
    aux_list->actual_camera->projection_type = PROJECTION_PERSPECTIVA;
    aux_list->actual_camera->angle = 45.0f;
    aux_list->actual_camera->near = 0.1f;
    aux_list->actual_camera->far = 100.0f;

    /* Pointer to first camera of the camera list */
    _camera_list_first = (list_camera *)malloc(sizeof(list_camera));
    _camera_list_first = aux_list;

    /* Set next camera */
    _camera_list_first->nextptr = 0;

    /* Set the selected camera pointer */
    _selected_camera = _camera_list_first;
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
    printf(" Insert verticality [x y z]: \n");
    scanf("%lf %lf %lf", &vup.x, &vup.y, &vup.z);

    printf(" Inserting camera...\n");

    aux_list = create_camera(pos, front, vup);

    printf(" Adding camera to list... \n");

    add_camera_to_list(aux_list);

    set_camera_projection(aux_list->actual_camera);

    printf(" Camera added to list.\n");
}