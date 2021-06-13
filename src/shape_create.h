#ifndef MECHANIC_PULSATION_SHAPE_CREATE_H
#define MECHANIC_PULSATION_SHAPE_CREATE_H

float float_rand(float mini, float maxi);
float normalization_value(float v[3]);
void add_vertex(float v[3], float points_array[], unsigned int *ip);
int vertexAlreadyExist(float v[3], float points_array[],  unsigned int *ip);
void add_to_arrays(float v[3], float points_array[], unsigned int index_array[],  unsigned int *ip, unsigned int *is);
void subdivide(float* v1, float* v2, float* v3, unsigned int SubdivisionCoef,
               float points_array[], unsigned int index_array[], unsigned int *ip, unsigned int *is);
void create_sphere(unsigned int nb_subdivision, float sphere_points_array[], unsigned int sphere_points_index[],
                   float icosahedron_points[], unsigned int icosahedron_index[],  unsigned int *ip, unsigned int *is);

#endif
