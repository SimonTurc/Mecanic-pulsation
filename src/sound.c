#include "sound.h"
#include <err.h>
#include <math.h>

int SamplesSum(int start, int end, Uint8 *buffer) {
  int n = 0; // Int to stock the sum
  while (start <= end) {
    n += buffer[start];
    start++;
  }
  return n;
}

void swap(int *a, int *b) {
  int temp = *a;
  *a = *b;
  *b = temp;
}

int partition(int *array, int low_index, int high_index) {
  int pivot = array[high_index]; // Value for which elements will be replaced in
                                 // the array
  int i = (low_index - 1);

  for (int j = low_index; j <= high_index - 1; j++) {
    // if current element is smaller than the pivot
    if (array[j] < pivot) {
      i++; //++ the index of the smallest element
      swap(&array[i], &array[j]);
    }
  }
  swap(&array[i + 1], &array[high_index]);
  return (i + 1);
}

// low_index = starting index
// high_index = ending index
void quickSort(int *array, int low_index, int high_index) {
  if (low_index < high_index) {
    // parv is partitioning index, array[parv] is now at the right place
    int parv = partition(array, low_index, high_index);

    // Now sort element before
    // and after parv
    quickSort(array, low_index, parv - 1);
    quickSort(array, parv + 1, high_index);
  }
}

float average(int *arr, int intsize) {
  float fintsize = (float)intsize;
  float average_value = 0.0;
  for (int i = 0; i < intsize; i++) {
    average_value += (float)arr[i];
  }
  average_value = average_value / fintsize;
  return average_value;
}

float variance(int *arr, int array_size, float average_value) {
  float variance = 0;
  for (int i = 0; i < array_size; i++) {
    variance +=
        ((float)arr[i] - average_value) * ((float)arr[i] - average_value);
    // printf("val %i %f \n", i, (float) arr[i] - average_value);
  }
  // printf("Variance1: %f \n", variance);
  variance = variance / (float)array_size;
  // printf("%i \n", array_size);
  return variance;
}

double ecart_type(int *arr, int array_size) {
  float average_value = average(arr, array_size);
  // printf("%f \n", average_value);
  float variance_value = variance(arr, array_size, average_value);
  // printf("Variance: %f \n", variance_value);
  double dvariance = (double)variance_value;
  double ecart_type_value = sqrtf(dvariance);
  return ecart_type_value;
}

/*float LowPassCoef()
{ // That calculates the coefficient that will be apply to each sample
  float RC = 1.0/(441 * 2 * M_PI);//Normalizing
  float dt = 1.0/44100; //Gives us the invert of the frequency
  float fBeta = dt/(RC+dt);//Creating the coefficient 0<fBeta<1 (fBeta for
FloatBeta) return fBeta;

}
void LowPassEdit(Uint8 *buffer, int length, float fBeta){//Edit the buffer of
sample float FinalBeta; if(length != 0) for(int i = 0; i < length; i++)
      {
        FinalBeta = fBeta * (float) buffer[i] + (1.0 - fBeta) * (float)
buffer[i-1];//Formula of low pass filter
        buffer[i] = (int) FinalBeta;
      }
}
void LowPassFilter(Uint8 *buffer, int length){// Just apply the filter
  float fBeta = LowPassCoef();
  printf("%f\n", fBeta);
  LowPassEdit(buffer, length, fBeta);
  }*/

float HighPassCoef() {
  // Calculates the coefficient applied to each sample
  float RC = 1.0 / (441 * 2 * M_PI);
  float dt = 1.0 / 44100;
  float falpha = RC / (RC + dt);
  return falpha;
}

void HighPassEdit(Uint8 *buffer, int length, float falpha) {
  float FinalAlpha;
  float temp = buffer[0];
  if (length != 0) {
    for (int i = 1; i < length; i++) {
      FinalAlpha = falpha * ((float)buffer[i - 1] + buffer[i] - temp);
      temp = buffer[i];
      buffer[i] = (int)FinalAlpha;
    }
  }
}

void HighPassFilter(Uint8 *buffer, int length) {
  float falpha = HighPassCoef();
  printf("Alpha: %f\n", falpha);
  HighPassEdit(buffer, length, falpha);
}

float build_ETV_value(int intsize, float *fullpulsation) {
  int median[intsize];
  for (int i = 0; i < intsize; i++)
    median[i] = fullpulsation[i];
  quickSort(median, 0, intsize - 1);
  // int median_value = median[intsize / 2];
  float ecart_type_value = ecart_type(median, intsize);
  return ecart_type_value;
}

void filter_lp(Uint8 *buffer)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    errx(EXIT_FAILURE, "Unable to initialize SDL: %s", SDL_GetError());
  }

  if (Mix_OpenAudio(44100, AUDIO_S8, 1, 1024) < 0) {
    SDL_Quit();
    errx(EXIT_FAILURE, "Unable to initialize SDL_mixer : %s", Mix_GetError());
  }

  Mix_Chunk *sound = Mix_LoadWAV_RW(SDL_RWFromFile("test_files/piano2.wav", "rb"), 1);
  if (sound == NULL) {
    Mix_CloseAudio();
    SDL_Quit();
    errx(EXIT_FAILURE, "Unable to load sound: %s", Mix_GetError());
  }
  int j = 0;
  for(Uint32 i = 0; i < sound->alen; i++)
    {
      if(i%44100 > 2000)
	{
	  buffer[j] = sound->abuf[i];
	  j++;
	}
    }
  printf("J: %i\n", j);
  Mix_FreeChunk(sound);
  Mix_CloseAudio();
  SDL_Quit();
}

void filter_hp(Uint8 *buffer)
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    errx(EXIT_FAILURE, "Unable to initialize SDL: %s", SDL_GetError());
  }

  if (Mix_OpenAudio(44100, AUDIO_S8, 1, 1024) < 0) {
    SDL_Quit();
    errx(EXIT_FAILURE, "Unable to initialize SDL_mixer : %s", Mix_GetError());
  }

  Mix_Chunk *sound = Mix_LoadWAV_RW(SDL_RWFromFile("test_files/piano2.wav", "rb"), 1);
  if (sound == NULL) {
    Mix_CloseAudio();
    SDL_Quit();
    errx(EXIT_FAILURE, "Unable to load sound: %s", Mix_GetError());
  }
  int j = 0;
  for(Uint32 i = 0; i < sound->alen; i++)
    {
      if(i%44100 < 42100)
	{
	  buffer[j] = sound->abuf[i];
	  j++;
	}
    }
  printf("J: %i\n", j);
  Mix_FreeChunk(sound);
  Mix_CloseAudio();
  SDL_Quit();
}

void pulsation_array(char *filename, float *result, int intsize) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    errx(EXIT_FAILURE, "Unable to initialize SDL: %s", SDL_GetError());
  }

  if (Mix_OpenAudio(44100, AUDIO_S8, 1, 1024) < 0) {
    SDL_Quit();
    errx(EXIT_FAILURE, "Unable to initialize SDL_mixer : %s", Mix_GetError());
  }

  Mix_AllocateChannels(1);
  Mix_Volume(0, MIX_MAX_VOLUME);

  Mix_Chunk *sound = Mix_LoadWAV_RW(SDL_RWFromFile(filename, "rb"), 1);
  if (sound == NULL) {
    Mix_CloseAudio();
    SDL_Quit();
    errx(EXIT_FAILURE, "Unable to load sound: %s", Mix_GetError());
  }
  // HighPassFilter(sound->abuf, sound->alen);
  int nbsamples = 8820;
  // float size = sound_time * 5;
  // Creating few arrays that will be used to create a smooth spike
  // Now we have to fill the array with the sum of each 441 samples.
  // Lastly print the array
  float max = 0;
  float min = 255;
  float fullpulsation[intsize];
  for (int i = 0; i < intsize; i++) {
    fullpulsation[i] =
        SamplesSum(i * nbsamples, (i + 1) * nbsamples, sound->abuf) / nbsamples;
    if (fullpulsation[i] > max)
      max = fullpulsation[i];
    if (fullpulsation[i] < min)
      min = fullpulsation[i];
  }
  float ETV = build_ETV_value(intsize, fullpulsation);
  // float coef = 0.756; // 0.378/0.5
  float dif = 0;
  if (fullpulsation[0] > 127)
    result[0] = 1 + ((fullpulsation[0] - 128) / (3.4 * (max - 128)));
  else
    result[0] = -1 - (0.3 - ((fullpulsation[0]) / (3.4 * (127 - min))));
  for (int i = 1; i < intsize; i++) {
    if (fullpulsation[i] > 127) {
      result[i] = 1.1 + ((fullpulsation[i] - 128) / (5 * (max - 128)));
      dif = fullpulsation[i] - fullpulsation[i - 1];
      if (dif < 0) {
        if (-(dif) > 0.5 * ETV)
          result[i] -= (dif / (max - min)) * 0.2;
      } else {
        if (dif > 0.5 * ETV)
          result[i] += (dif / (max - min)) * 0.2;
      }
    } else {
      result[i] = -1.1 - (0.3 - ((fullpulsation[i]) / (5 * (127 - min))));
      dif = fullpulsation[i] - fullpulsation[i - 1];
      if (dif < 0) {
        if (-(dif) > 0.5 * ETV)
          result[i] -= (dif / (max - min)) * 0.2;
      } else {
        if (dif > 0.5 * ETV)
          result[i] += (dif / (max - min)) * 0.2;
      }
    }
  }

  Mix_FreeChunk(sound);
  Mix_CloseAudio();
  SDL_Quit();
}

void play_sound(char *file, float len) {
  int ilen = (int)(len*42300);
  printf("%i\n", ilen);
  Uint8 buffer[ilen];
  //filter_lp(buffer);
  filter_hp(buffer);
  //Faire un if qui check quel bouton GTK est activé et load le son selon ca
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    errx(EXIT_FAILURE, "Unable to initialize SDL: %s", SDL_GetError());
  }

  if (Mix_OpenAudio(42100, AUDIO_S8, 1, 1024) < 0) {
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
  //Idem si le bouton GTK est allumé pr 
  //for(Uint32 i = 0; i < sound->alen; i++)
  //{
  //  sound->abuf[i] = buffer[i];
  //}
  /*264600*/
  for(int i = 0; i < ilen; i++)
  {
     sound->abuf[i] = buffer[i];
  }
  //HighPassFilter(sound->abuf, sound->alen);
  if (Mix_PlayChannel(0, sound, 0) == -1) {
    Mix_CloseAudio();
    SDL_Quit();
    errx(EXIT_FAILURE, "Unable to play on the channel 0 : %s", Mix_GetError());
    }

  /*FILE *fptr;
  fptr = fopen("value.csv","w");
  //fprintf(fptr,"[");
  for (Uint32 i = 0; i < 20000; ++i) {
    if (i == (sound->alen) - 1)
      fprintf(fptr, "%d", sound->abuf[i]);
    else
      fprintf(fptr, "%d,", sound->abuf[i]);
  }
  //fprintf(fptr,"]");
  fclose(fptr);*/
  // HighPassFilter(sound->abuf, sound->alen);
  while (Mix_Playing(0)) {
  }

  Mix_FreeChunk(sound);
  Mix_CloseAudio();
  SDL_Quit();
}
/*
int main(int argc, char *argv[]) {
  if (argc != 2)
    errx(EXIT_FAILURE, "./sound [path]");
  play_sound(argv[1]);
  return EXIT_SUCCESS;
  }
*/
