#include <math.h>
#include <stdio.h>

void on_motion(float point[],int nb_point)
// Origin is 0 , 0 , 0
{
    float angle = M_PI/180; // angle de rotation en degré
    for (int i = 0; i < nb_point; i++)
    {
        if (point[i* 6 ] != 0 || point[i* 6 + 2] != 0)
        {
            point[i*6] = point[i*6]* cosf(angle)- point[i*6+2] * sinf(angle);
            point[i*6 + 2]= point[i*6] *sinf(angle)+ point[i*6+2]* cosf(angle);
        } 
    }
}