#include <err.h>
#include <math.h>
#include "sound.h"

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

void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

int partition(int *array, int low_index, int high_index)
{
  int pivot = array[high_index];//Value for which elements will be replaced in the array
  int i = (low_index -1);

  for(int j = low_index; j <= high_index - 1; j++)
    {
      //if current element is smaller than the pivot
      if(array[j] < pivot)
	{
	  i++;//++ the index of the smallest element
	  swap(&array[i], &array[j]);
	}
    }
  swap(&array[i+1], &array[high_index]);
  return (i+1);
  
}

//low_index = starting index
//high_index = ending index
void quickSort(int *array, int low_index, int high_index)
{
  if(low_index < high_index)
    {
      //parv is partitioning index, array[parv] is now at the right place
      int parv = partition(array, low_index, high_index);

      //Now sort element before
      //and after parv
      quickSort(array, low_index,parv-1);
      quickSort(array, parv+1, high_index);
    }
  	
}

float average(int *arr, int intsize)
{
  float fintsize = (float) intsize;
  float average_value = 0.0;
  for(int i = 0; i < intsize; i++)
    {
      average_value += (float) arr[i];
    }
  average_value= average_value/fintsize;
  return average_value;
}

float variance(int *arr, int array_size, float average_value)
{
  float variance = 0;
  for(int i=0; i < array_size; i++)
    {
      variance += ((float) arr[i] - average_value) * ((float) arr[i] - average_value);
    }
  variance = variance/(float) array_size;
  return variance;
}

double ecart_type(int *arr, int array_size)
{
  float average_value = average(arr, array_size);
  printf("%f \n", average_value);
  float variance_value = variance(arr, array_size, average_value);
  printf("Variance: %f \n", variance_value);
  double dvariance = (double) variance_value;
  double ecart_type_value = sqrtf(dvariance);
  return ecart_type_value;
}

/*float LowPassCoef()
{ // That calculates the coefficient that will be apply to each sample
  float RC = 1.0/(441 * 2 * M_PI);//Normalizing
  float dt = 1.0/44100; //Gives us the invert of the frequency
  float fBeta = dt/(RC+dt);//Creating the coefficient 0<fBeta<1 (fBeta for FloatBeta)
  return fBeta;
  
}

void LowPassEdit(Uint8 *buffer, int length, float fBeta){//Edit the buffer of sample 
  float FinalBeta;
  if(length != 0)
    for(int i = 0; i < length; i++)
      {
	FinalBeta = fBeta * (float) buffer[i] + (1.0 - fBeta) * (float) buffer[i-1];//Formula of low pass filter
	buffer[i] = (int) FinalBeta;  
      }
}

void LowPassFilter(Uint8 *buffer, int length){// Just apply the filter
  float fBeta = LowPassCoef();
  printf("%f\n", fBeta);
  LowPassEdit(buffer, length, fBeta);
  }*/

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

  printf("%i\n", sound->alen);
  float sound_time = sound->alen/(44100.0);// Durée de l'audio
 
  //If I have 44100 samples per sec, and I want 100 values for 1 sec
  //It means that there are 441samples per value
  //We can expect 630 values for piano2.wav
  
  float size = sound_time * 100;//Basically this is the number of int that wwe will get through the array
  int intsize = (int) size;//Convert the float size to a int to create array 

  //Creating few arrays that will be used to create a smooth spike 
  int fullpulsation[intsize];//Principal pulsation
  //int semipulsation[intsize];//Semi pulsation
  //int thirthpulsation[intsize];//Thirth pulsation 
  //int quarterpulsation[intsize];//Quarter pulsation

  //printf("Before %i -> %i \n", sound->abuf[50000], sound->abuf[50001]); 
  //LowPassFilter(sound->abuf, sound->alen);// We apply the filter to the array of samples.
  //printf("Before %i -> %i \n", sound->abuf[50000], sound->abuf[50001]);
  for (Uint32 i = 0; i < sound->alen;++i) {
    if(i > 100000 && i < 100300)
      printf("LowPassFilter: %i ; Samples: %i\n", i, sound->abuf[i]);
  }
  printf("%i\n", sound->alen);
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
      //semipulsation[i] = fullpulsation[i]/2;
      //thirthpulsation[i] = fullpulsation[i]/3;
      //quarterpulsation[i] = fullpulsation[i]/4;
      }
  
  int median[intsize];
  for(int i = 0; i < intsize; i++)
    median[i] = fullpulsation[i];
  quickSort(median, 0, intsize-1);
  for(int i = 0; i < intsize; i++)
    {
      printf("Index: %i, Value: %i \n", i, median[i]);
    }
  int median_value = median[intsize/2];
  printf("%i \n", median_value);
  float ecart_type_value = ecart_type(median, intsize);
  printf("ETV: %f \n", ecart_type_value);
  /*FILE *fptr;
  fptr = fopen("value.txt","w");
  fprintf(fptr,"[");
  for (Uint32 i = 0; i < 20000; ++i) {
    if (i == (sound->alen) - 1)
      fprintf(fptr, "%d", sound->abuf[i]);
    else
      fprintf(fptr, "%d,", sound->abuf[i]);
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
