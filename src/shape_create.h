#ifndef MECHANIC_PULSATION_SHAPE_CREATE_H
#define MECHANIC_PULSATION_SHAPE_CREATE_H

const float a = 0.525731112119133606; // (1 / sqrt(1 +(1 + sqrt(5))/2)²)
const float b = 0.850650808352039932; // ((1 + sqrt(5))/2) / sqrt(1 +(1 + sqrt(5))/2)²)

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
