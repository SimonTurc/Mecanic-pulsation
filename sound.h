#ifndef MECHANIC_PULSATION_SOUND_H
#define MECHANIC_PULSATION_SOUND_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

int SamplesSum(int start, int end, Uint8 *buffer); // Sum 441 samples

float LowPassCoef();                                      // Calculate coefficient for LowPassFilter
void LowPassEdit(Uint8 *buffer, int length, float fBeta); // Edit the buffer of samples

void LowPassFilter(Uint8 *buffer, int length);

void play_sound(char *file);

#endif // MECHANIC_PULSATION_SOUND_H
