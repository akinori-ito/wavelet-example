#include <stdio.h>
#include <stdlib.h>
#include "wavio.h"

int myrand(int mag)
{
  return random()%(mag*2)-mag;

}

int main(int argc, char *argv[])
{
  int ret,i,mag;
  SoundData *s;

  if (argc < 4) {
    fprintf(stderr,"usage: %s magnitude infile outfile\n",argv[0]);
    exit(1);
  }

  mag = atoi(argv[1]);
  if ((s = readWavFile(argv[2],&ret)) == NULL) {
    fprintf(stderr,"Error: %s:  %s\n",argv[2],SndStrError(ret));
    exit(1);
  }
  for (i = 0; i < s->size; i++) {
    s->data[0][i] += myrand(mag);
  }
  if ((ret = writeWavFile(s,argv[3])) != SND_OK) {
    fprintf(stderr,"Error: %s:  %s\n",argv[3],SndStrError(ret));
    exit(1);
  }
  return 0;
}
