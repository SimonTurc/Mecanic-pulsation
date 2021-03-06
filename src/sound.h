#ifndef MECHANIC_PULSATION_SOUND_H
#define MECHANIC_PULSATION_SOUND_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

int SamplesSum(int start, int end, Uint8 *buffer); // Sum 441 samples

void swap(int *a, int *b);

int partition(int *array, int low_index, int high_index);

void quickSort(int *array, int low_index, int high_index);

float average(int *arr, int intsize);

float variance(int *arr, int array_size, float average_value);

double ecart_type(int *arr, int array_size);

float LowPassCoef(); // Calculate coefficient for LowPassFilter

void LowPassEdit(Uint8 *buffer, int length, float fBeta); // Edit the buffer of samples

void LowPassFilter(Uint8 *buffer, int length);

float HighPassCoef();

void HighPassEdit(Uint8 *buffer, int length, float falpha);

void HighPassFilter(Uint8 *buffer, int length);

float build_ETV_value(int intsize, float *fullpulsation);

void pulsation_array(char *filename, float *result, int intsize, int state);

void play_sound(char *file, float len, int state);

#endif // MECHANIC_PULSATION_SOUND_H
