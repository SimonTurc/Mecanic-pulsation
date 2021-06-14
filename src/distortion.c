#include "movement.h"
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

unsigned int high_icosphere[] = {5,  9,  11, 12, 13, 14, 18, 19, 20,  21, 23,
                                 24, 25, 26, 27, 28, 29, 30, 31, 32,  33, 34,
                                 35, 36, 37, 38, 39, 40, 41, 42, 43,  44, 45,
                                 50, 71, 72, 73, 74, 77, 78, 79, 156, 157};

unsigned int low_icosphere[] = {
    57,  89,  91,  93,  94,  95,  96,  97,  98,  99,  101, 102, 103, 104, 105,
    106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120,
    121, 122, 124, 126, 127, 128, 133, 134, 135, 138, 139, 159, 160};

unsigned int high_icosahedron[] = {4, 5, 8, 9};

unsigned int low_icosahedron[] = {6, 7, 10, 11};

void distortion_shape(float *deformation_factors,
                      unsigned int deformation_length, float points_array[],
                      unsigned int nb_points) {
  for (unsigned int i = 0; i < deformation_length; i++) {
    unsigned int randomPoint = rand() % (nb_points - 2);
    randomPoint += 1;
    unsigned int randomPointUp = randomPoint + 1;
    unsigned int randomPointDown = randomPoint - 1;
    
    int p = 0;
    int pUp = 0;
    int pDown = 0;
    if (nb_points == 4) {
      if (deformation_factors[i] > 0) {
        p = high_icosahedron[randomPoint];
        pUp = high_icosahedron[randomPointUp];
        pDown = high_icosahedron[randomPointDown];
      } else {
        deformation_factors[i] = -deformation_factors[i];
        p = low_icosahedron[randomPoint];
        pUp = low_icosahedron[randomPointUp];
        pDown = low_icosahedron[randomPointDown];
      }
    } else if (nb_points == 43) {
      if (deformation_factors[i] > 0) {
        p = high_icosphere[randomPoint];
        pUp = high_icosphere[randomPointUp];
        pDown = high_icosphere[randomPointDown];
      } else {
        deformation_factors[i] = -deformation_factors[i];
        p = low_icosphere[randomPoint];
        pUp = low_icosphere[randomPointUp];
        pDown = low_icosphere[randomPointDown];
      }
    } else {
      return;
    }

    float x = sqrtf(deformation_factors[i]);
    deformation(points_array, p, x);

    if (randomPoint > 0 && randomPoint < nb_points) {
      deformation(points_array, pUp, x);
      deformation(points_array, pDown, x);
    }

    usleep(100000);
    deformation(points_array, p, x);
    usleep(100000);

    deformation(points_array, p, 1 / deformation_factors[i]);
    if (randomPoint > 0 && randomPoint < nb_points) {
      deformation(points_array, pUp, 1 / x);
      deformation(points_array, pDown, 1 / x);
    }
  }
}
