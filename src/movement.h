#ifndef MECHANIC_PULSATION_MOVEMENT_H
#define MECHANIC_PULSATION_MOVEMENT_H

void deformation(float point[], int index_point, float deformation_factor);
void on_motion(float point[], int nb_point);
void scaling(float point[], int nb_point, float scaling_factor);
void high(float point[], int index_point);
void slide(float point[], int nb_point, int start);

#endif // MECHANIC_PULSATION_SHAPE_H
