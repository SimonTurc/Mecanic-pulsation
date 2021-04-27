#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <err.h>
#include <math.h>

int SamplesSum(int start, int end, Uint8 *buffer)
{
  int n = 0;//Int to stock the sum
  while(start <= end)
    {
      n += buffer[start];
      start++;
    }
  return n;
}

void play_sound(char *file) {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    errx(EXIT_FAILURE, "Unable to initialize SDL: %s", SDL_GetError());
  }

  if (Mix_OpenAudio(44100, AUDIO_S8, 1, 1024) <
      0) {
    SDL_Quit();
    errx(EXIT_FAILURE, "Unable to initialize SDL_mixer : %s", Mix_GetError());
  }

  Mix_AllocateChannels(1);
  Mix_Volume(0, MIX_MAX_VOLUME);

  Mix_Chunk *sound = Mix_LoadWAV_RW(SDL_RWFromFile(file, "rb"), 1);
  if (sound == NULL) {
    Mix_CloseAudio();
    SDL_Quit();
    errx(EXIT_FAILURE, "Unable to load sound: %s", Mix_GetError());
  }
  if (Mix_PlayChannel(0, sound, 0) == -1) {
    Mix_CloseAudio();
    SDL_Quit();
    errx(EXIT_FAILURE, "Unable to play on the channel 0 : %s", Mix_GetError());
  }
  int nbsamples = 441;//nombre de samples par index du tableau (44100/100ms)

  float sound_time = sound->alen/44100.0;// Durée de l'audio
 
  //If I have 44100 samples per sec, and I want 100 values for 1 sec
  //It means that there are 441samples per value
  //We can expect 630 values for piano2.wav
  
  float size = sound_time * 100;//Basically this is the number of int that wwe will get through the array
  int intsize = (int) size;//Convert the float size to a int to create array 

  //Creating few arrays that will be used to create a smooth spike 
  int fullpulsation[intsize];//Principal pulsation
  int semipulsation[intsize];//Semi pulsation
  int thirthpulsation[intsize];//Thirth pulsation 
  int quarterpulsation[intsize];//Quarter pulsation

  //Time to edit the abuf to get a new audio
  for (Uint32 i = 0; i < sound->alen;++i) {
    if(i > 100000 && i < 120000)
      printf("Index_1: %i ; Samples: %d\n", i, sound->abuf[i]);
    double x = (double) sound->abuf[i];
    double y = x*sin(2.3);  
    sound->abuf[i] = (int) y;
    if(i > 100000 && i < 120000)
      printf("Index_2: %i ; Samples: %d\n", i, sound->abuf[i]);
    
  }

  //Now we have to fill the array with the sum of each 441 samples.
  printf("Audio time: %f \nSize: %f \nReal_size: %i\n", sound_time, size, intsize);
  //First a loop starting at 0 to intsize
  //Then call a function that will sum 441 samples from n*441 to n+1*441
  //Obviously, there will be few loses of data approximatly < 0.01
  //Lastly print the array
  //Furthermore to get a nice curve, we can take the same array but each value will get divided by 2 and 3 and 4 maybe
  for(int i=0; i < intsize; i++)
    {
      fullpulsation[i] = SamplesSum(i*441, (i+1)*441, sound->abuf)/nbsamples;
      semipulsation[i] = fullpulsation[i]/2;
      thirthpulsation[i] = fullpulsation[i]/3;
      quarterpulsation[i] = fullpulsation[i]/4;
    }
  /*for(int j=0; j < intsize; j++)
    {
      printf("Index: %i -> FullSum = %i ; SemiSum = %i ; ThirthSum = %i ; QuarterSum = %i\n", j, fullpulsation[j], semipulsation[j], thirthpulsation[j], quarterpulsation[j]);
    }*/
  //To print samples
  /*for (Uint32 i = 0; i < sound->alen;++i) {
      printf("Samples: %d\n", sound->abuf[i]);
      }*/
  /*FILE *fptr;
  fptr = fopen("value.txt","w");
  fprintf(fptr,"[");
  for (Uint32 i = 0; i < 630; ++i) {
    if (i == (sound->alen) - 1)
      fprintf(fptr, "%d", fullpulsation[i]);
    else
      fprintf(fptr, "%d,", fullpulsation[i]);
  }
  fprintf(fptr,"]");

  fclose(fptr);*/

  while (Mix_Playing(0)) {
  }

  Mix_FreeChunk(sound);
  Mix_CloseAudio();
  SDL_Quit();
}

/*int main(int argc, char *argv[]) {
  if (argc != 2)
    errx(EXIT_FAILURE, "./sound [path]");

  play_sound(argv[1]);
  return EXIT_SUCCESS;
}*/
