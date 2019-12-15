#include "definitions.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

extern object3d *_selected_object;

void liberar_memoria_obj(object3d *objptr)
{
    int i;

    // LIBERAR tabla de vertices
    free(objptr->vertex_table);

    // LIBERAR cada una de las entradas de las caras, recorriendo una a una
    for (i = 0; i < objptr->num_faces; i++)
    {
        free(objptr->face_table[i].vertex_table);
    }

    // LIBERAR tabla de caras
    free(objptr->face_table);

    // LIBERAR objeto
    free(objptr);
}

void set_identity(GLfloat *m)
{
    int i, j;

    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
        {
            m[i * 4 + j] = 0;
            if (i == j)
                m[i * 4 + j] = 1;
        }
}

void print_4x4_matrix(GLfloat *m)
{
    int i, j;
    printf("\n");
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            printf("%f \t", m[4 * i + j]);
        }
        printf("\n");
    }
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

void set_normal_vectors() 
{
    GLint f, v, v_index, i;
    GLint i1, i2, i3;
    GLfloat norma;
    vector3 v_normal;
    vector3 v_normal_init = (vector3){.x = 0, .y = 0, .z = 0};

    for (i = 0; i < _selected_object->num_vertices; i++) 
    {
        _selected_object->vertex_table[i].normal_vector = v_normal_init;
    }

    for (f = 0; f < _selected_object->num_faces; f++)
    {
        i1 = _selected_object->face_table[f].vertex_table[0];
        i2 = _selected_object->face_table[f].vertex_table[1];
        i3 = _selected_object->face_table[f].vertex_table[2];

        v_normal = calc_vect_normal(_selected_object->vertex_table, i1, i2, i3);

        _selected_object->face_table[f].normal_vector = v_normal;

        norma = sqrt(
            pow(_selected_object->face_table[f].normal_vector.x, 2) +
            pow(_selected_object->face_table[f].normal_vector.y, 2) +
            pow(_selected_object->face_table[f].normal_vector.z, 2));

        _selected_object->face_table[f].normal_vector.x /= norma;
        _selected_object->face_table[f].normal_vector.y /= norma;
        _selected_object->face_table[f].normal_vector.z /= norma;

        for (v = 0; v < _selected_object->face_table[f].num_vertices; v++)
        {
            v_index = _selected_object->face_table[f].vertex_table[v];
            _selected_object->vertex_table[v_index].normal_vector.x += _selected_object->face_table[f].normal_vector.x;
            _selected_object->vertex_table[v_index].normal_vector.y += _selected_object->face_table[f].normal_vector.y;
            _selected_object->vertex_table[v_index].normal_vector.z += _selected_object->face_table[f].normal_vector.z;
        }
    }

    for (i = 0; i < _selected_object->num_vertices; i++) 
    {
        norma = sqrt(
            pow(_selected_object->vertex_table[i].normal_vector.x, 2) +
            pow(_selected_object->vertex_table[i].normal_vector.y, 2) +
            pow(_selected_object->vertex_table[i].normal_vector.z, 2));

        _selected_object->vertex_table[i].normal_vector.x /= norma;
        _selected_object->vertex_table[i].normal_vector.y /= norma;
        _selected_object->vertex_table[i].normal_vector.z /= norma;
    }
}