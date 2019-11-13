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

extern list_camera *_camera_list_first;
extern list_camera *_selected_camera;

void add_camera(vector3 camera_pos, vector3 camera_front, vector3 camera_up)
{
    camera *cm = (camera*)malloc(sizeof(camera));
    GLfloat *inv = (GLfloat *)malloc(sizeof(GLfloat) * 16);
    list_camera *aux_list = (list_camera*)malloc(sizeof(list_camera));

    if (_camera_list_first == 0) // if not camera, create first
    {
        /* Pointer to first camera of the camer lista */
        _camera_list_first = (list_camera *)malloc(sizeof(list_camera));

        _camera_list_first->actual_camera = cm;

        /* Set next camera */
        _camera_list_first->nextptr = 0;

        _selected_camera = _camera_list_first;
    } 
    else
    {
        aux_list->nextptr = 0;
        _selected_camera->nextptr = aux_list;
        _selected_camera = aux_list;
    }
    
    
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

    glGetFloatv(GL_MODELVIEW_MATRIX, _selected_camera->actual_camera->m);

    /*
    xc.x xc.y xc.z -e.xc
    yc.x yc.y yc.z -e.yc
    zc.x zc.y zc.z -e.zc
    0    0    0   1
    */

   // reorganize for the m inverse
    _selected_camera->actual_camera->m_inv[0] = _selected_camera->actual_camera->m[0];
    _selected_camera->actual_camera->m_inv[1] = _selected_camera->actual_camera->m[4];
    _selected_camera->actual_camera->m_inv[2] = _selected_camera->actual_camera->m[8];
    _selected_camera->actual_camera->m_inv[3] = camera_pos.x;
    _selected_camera->actual_camera->m_inv[4] = _selected_camera->actual_camera->m[1];
    _selected_camera->actual_camera->m_inv[5] = _selected_camera->actual_camera->m[5];
    _selected_camera->actual_camera->m_inv[6] = _selected_camera->actual_camera->m[9];
    _selected_camera->actual_camera->m_inv[7] = camera_pos.y;
    _selected_camera->actual_camera->m_inv[8] = _selected_camera->actual_camera->m[2];
    _selected_camera->actual_camera->m_inv[9] = _selected_camera->actual_camera->m[6];
    _selected_camera->actual_camera->m_inv[10] = _selected_camera->actual_camera->m[10];
    _selected_camera->actual_camera->m_inv[11] = camera_pos.z;
    _selected_camera->actual_camera->m_inv[12] = 0;
    _selected_camera->actual_camera->m_inv[13] = 0;
    _selected_camera->actual_camera->m_inv[14] = 0;
    _selected_camera->actual_camera->m_inv[15] = 1;
}

void set_default_cameras()
{
    vector3 cam_pos = (vector3){ .x = 2.0f, .y = 1.0f, .z = 4.0f };
    vector3 cam_front = (vector3){ .x = 0.0f, .y = 0.0f, .z = -1.0f };
    vector3 cam_vup = (vector3){ .x = 0.0f, .y = 1.0f, .z = 0.0f };
    
    add_camera(cam_pos, cam_front, cam_vup);
}