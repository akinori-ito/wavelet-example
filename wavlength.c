#include <stdio.h>
#include <stdlib.h>
#include "wavio.h"

int main(int argc, char *argv[])
{
  SoundData *wav;
  int ret_val;
  if (argc < 2) {
    fprintf(stderr,"Usage: wavlength wavfilename\n");
    exit(1);
  }
  wav = readWavFile(argv[1],&ret_val);
  if (wav == NULL) {
    fprintf(stderr,"Failed to read wav file\n");
    exit(1);
  }
  printf("%f\n",(double)wav->size/wav->header.fmt.samplerate);
  return 0;
}