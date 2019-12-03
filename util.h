#include "definitions.h"

void liberar_memoria_obj(object3d *objptr);
void set_identity(GLfloat *m);
void print_4x4_matrix(GLfloat *m);
vector3 calc_vect_normal(vertex *vertex_table, GLint i1, GLint i2, GLint i3);