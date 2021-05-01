#include <math.h>
#include <stdio.h>

void on_motion(float point[],int nb_point)
// Origin is 0 , 0 , 0
{
    float angle = (2*M_PI)/180; // angle de rotation en degré
    float alpha = cos(angle);
    float beta = sin(angle);
    float a ;
    if (cos(angle)<0)
    {
      a = -0.0006;
    }
    else
    {
      a = 0.0006;
    }
    for (int i = 0; i < nb_point; i++)
    {
      point[i*6] = point[i*6]* (alpha+a)+ point[i*6+2] * (beta-a);
      point[i*6 + 2]= -point[i*6] *(beta-a)+ point[i*6+2]* (alpha+a);
    }
}

void scaling(float point[],int nb_point, float scaling_factor)
{
    for (int i = 0; i < nb_point; i++)
    {
        point[i * 6] = point[i*6] * scaling_factor;
        point[i * 6 + 1] = point[i * 6 + 1] * scaling_factor;
        point[i * 6 + 2]= point[i * 6 +2] * scaling_factor;
    }
}


void deformation(float point[],int index_point, float deformation_factor)
{
  point[index_point * 6] = point[index_point * 6] * deformation_factor;
  point[index_point * 6 + 1] = point[index_point * 6 + 1] * deformation_factor;
  point[index_point * 6 + 2]= point[index_point * 6 +2] * deformation_factor;
}
