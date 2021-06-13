#include "movement.h"
#include <pthread.h>
#include <math.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>


void distortion_shape(float *deformation_factors,unsigned int deformation_length, float points_array[], unsigned int nb_points){
   for(unsigned int i = 0; i < deformation_length; i++){
     unsigned int randomPoint = rand() % (nb_points - 1);
     float x = sqrtf(deformation_factors[i]);
     deformation(points_array, randomPoint, x);
     
     if(randomPoint > 0 && randomPoint < nb_points){
       deformation(points_array, randomPoint + 1, x);
       deformation(points_array, randomPoint - 1, x);
     }
    
     usleep(100000);
     deformation(points_array, randomPoint, x);
     usleep(100000);
     
     deformation(points_array, randomPoint, 1/deformation_factors[i]);
     if(randomPoint > 0 && randomPoint < nb_points){
       deformation(points_array, randomPoint + 1, 1/x);
       deformation(points_array, randomPoint - 1, 1/x);
     }
   }
}


