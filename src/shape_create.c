#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float float_rand(float mini, float maxi) {
  float scale = rand() / (float)RAND_MAX;
  return mini + scale * (maxi - mini);
}

float normalization_value(float v[3]) {
  float n = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  return n;
}

void add_vertex(float v[3], float points_array[], unsigned int *ip) {
  for (int i = 0; i < 3; i++) {
    points_array[*ip + i] = v[i];
  }
  *ip += 3;
  // Add color
  for (int i = 0; i < 3; i++) {
    float x = rand() / (float)RAND_MAX;
    points_array[*ip + i] = x;
  }
  *ip += 3;
}

int vertexAlreadyExist(float v[3], float points_array[], unsigned int *ip) {
  for (unsigned int i = 0; i < *ip / 6; i++) {
    if (v[0] == points_array[i * 6]) {
      if (v[1] == points_array[i * 6 + 1]) {
        if (v[2] == points_array[i * 6 + 2]) {
          return i;
        }
      }
    }
  }
  return -1;
}

void add_to_arrays(float v[3], float points_array[], unsigned int index_array[],
                   unsigned int *ip, unsigned int *is) {
  int current_index = vertexAlreadyExist(v, points_array, ip);

  if (current_index == -1) {
    index_array[*is] = *ip / 6;
    add_vertex(v, points_array, ip);
  } else {
    index_array[*is] = current_index;
  }
  *is += 1;
}

void subdivide(float *v1, float *v2, float *v3, unsigned int SubdivisionCoef,
               float points_array[], unsigned int index_array[],
               unsigned int *ip, unsigned int *is) {
  float v12[3], v23[3], v31[3];
  float n12, n23, n31;

  if (SubdivisionCoef == 0) {
    add_to_arrays(v1, points_array, index_array, ip, is);
    add_to_arrays(v2, points_array, index_array, ip, is);
    add_to_arrays(v3, points_array, index_array, ip, is);
    return;
  }

  for (int i = 0; i < 3; i++) {
    v12[i] = (v1[i] + v2[i]) / 2.0;
    v23[i] = (v2[i] + v3[i]) / 2.0;
    v31[i] = (v3[i] + v1[i]) / 2.0;
  }

  n12 = normalization_value(v12);
  n23 = normalization_value(v23);
  n31 = normalization_value(v31);

  for (int j = 0; j < 3; j++) {
    v12[j] /= n12;
    v23[j] /= n23;
    v31[j] /= n31;
  }
  subdivide(v1, v12, v31, SubdivisionCoef - 1, points_array, index_array, ip,
            is);
  subdivide(v2, v23, v12, SubdivisionCoef - 1, points_array, index_array, ip,
            is);
  subdivide(v3, v31, v23, SubdivisionCoef - 1, points_array, index_array, ip,
            is);
  subdivide(v12, v23, v31, SubdivisionCoef - 1, points_array, index_array, ip,
            is);
}

void create_sphere(unsigned int nb_subdivision, float sphere_points_array[],
                   unsigned int sphere_points_index[],
                   float icosahedron_points[], unsigned int icosahedron_index[],
                   unsigned int *ip, unsigned int *is) {
  float v1[3];
  float v2[3];
  float v3[3];
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 3; j++) {
      v1[j] = icosahedron_points[icosahedron_index[i * 3] * 6 + j];
      v2[j] = icosahedron_points[icosahedron_index[i * 3 + 1] * 6 + j];
      v3[j] = icosahedron_points[icosahedron_index[i * 3 + 2] * 6 + j];
    }
    subdivide(v1, v2, v3, nb_subdivision, sphere_points_array,
              sphere_points_index, ip, is);
  }
}
