#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <err.h>

void play_sound(char *file) {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    errx(EXIT_FAILURE, "Unable to initialize SDL: %s", SDL_GetError());
  }

  if (Mix_OpenAudio(96000, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) <
      0) {
    SDL_Quit();
    errx(EXIT_FAILURE, "Unable to initialize SDL_mixer : %s", Mix_GetError());
  }

  Mix_AllocateChannels(1);
  Mix_Volume(0, MIX_MAX_VOLUME);

  Mix_Chunk* sound = Mix_LoadWAV_RW(SDL_RWFromFile(file, "rb"), 1);

  Mix_PlayChannel(0, sound, 0);

  // To print samples
  /*for (Uint32 i = 0; i < sound->alen;++i) {
      printf("Samples: %d\n", sound->abuf[i]);
  }*/

  while(Mix_Playing(0)) { }

  Mix_FreeChunk(sound);
  Mix_CloseAudio();
  SDL_Quit();
}

int main(int argc, char *argv[]) {
  if (argc != 2)
    errx(EXIT_FAILURE, "./sound [path]");

  play_sound(argv[1]);
  return EXIT_SUCCESS;
}
