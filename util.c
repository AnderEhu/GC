#include "definitions.h"
#include <stdlib.h>
#include <stdio.h>

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
            if (i == j) m[i * 4 + j] = 1;
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