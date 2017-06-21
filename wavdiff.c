#include <stdio.h>
#include <stdlib.h>
#include "wavio.h"

int main(int argc, char *argv[])
{
  SoundData *s1, *s2;
  int i,ret;

  if (argc < 3) {
    fprintf(stderr,"usage: wavdiff infile1 infile2 outfile\n");
    exit(1);
  }
  if ((s1 = readWavFile(argv[1],&ret)) == NULL) {
    fprintf(stderr,"%s: %s\n",argv[1],SndStrError(ret));
    exit(1);
  }
  if ((s2 = readWavFile(argv[2],&ret)) == NULL) {
    fprintf(stderr,"%s: %s\n",argv[2],SndStrError(ret));
    exit(1);
  }

  for (i = 0; i < s1->size && i < s2->size; i++) {
    s1->data[0][i] -= s2->data[0][i];
  }

  writeWavFile(s1,argv[3]);
  return 0;
}
