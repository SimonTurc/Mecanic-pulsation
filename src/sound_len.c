#include <err.h>
#include <math.h>
#include "sound_len.h"

void get_sound_len(char *file, float *ptr) {

  /*if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    errx(EXIT_FAILURE, "Unable to initialize SDL: %s", SDL_GetError());
  }*/

  if (Mix_OpenAudio(44100, AUDIO_S8, 1, 1024) <
      0) {
    SDL_Quit();
    errx(EXIT_FAILURE, "Unable to initialize SDL_mixer : %s", Mix_GetError());
  }

  Mix_Chunk *sound = Mix_LoadWAV_RW(SDL_RWFromFile(file, "rb"), 1);
  if (sound == NULL) {
    Mix_CloseAudio();
    SDL_Quit();
    errx(EXIT_FAILURE, "Unable to load sound: %s", Mix_GetError());
  }
  

  *ptr = sound->alen/(44100.0);// Durée de l'audio

  Mix_FreeChunk(sound);
  Mix_CloseAudio();
  SDL_Quit();

}
/*
int main(int argc, char *argv[]) {
  if (argc != 2)
    errx(EXIT_FAILURE, "./sound [path]");
  float x;
  float *ptr = &x;
  get_sound_len(argv[1], ptr);
  printf("Audio_time: %f\n", *ptr);
  return *ptr;
  }
*/
