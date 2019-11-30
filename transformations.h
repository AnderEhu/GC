#include "definitions.h"

void init_transf_values();
void transf_matrix_init();
void transf_matrix_set(list_matrix *n_elem_ptr);
void transform(transf_values *values);
void analysis_mode(int x, int y);
GLfloat distance_camera_to_obj();