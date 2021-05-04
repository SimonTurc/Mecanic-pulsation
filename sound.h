#ifndef MECHANIC_PULSATION_SOUND_H
#define MECHANIC_PULSATION_SOUND_H

int SamplesSum(int start, int end, Uint8 *buffer); //Sum 441 samples

void swap(int *a, int* b);

int partition(int *array, int low_index, int high_index);

void quickSort(int *array, int low_index, int high_index);

float average(int *arr, int intsize);
float ecart_type(int *arr, int array_size);

float LowPassCoef();//Calculate coefficient for LowPassFilter
void LowPassEdit(Uint8 *buffer, int length, float fBeta);//Edit the buffer of samples

void LowPassFilter(Uint8 *buffer, int length);

void play_sound(char *file);

#endif //MECHANIC_PULSATION_SOUND_H
