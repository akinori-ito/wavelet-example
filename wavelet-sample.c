#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "wavio.h"
#include "wavelet.h"

/* real processing */
void process(SoundData *s)
{
  int length = s->size;
  double *org,*dst;
  double *coef;
  double max;
  int i;

  prepare(DAUBECHIES4);

  if (length % 2 != 0) {
    length--;
    s->data[0][length-1] = 0;
  }
  org = (double*)malloc(sizeof(double)*length);
  dst = (double*)malloc(sizeof(double)*length);
  coef = (double*)malloc(sizeof(double)*length);
  for (i = 0; i < length; i++) {
    org[i] = s->data[0][i];
  }
  decompose(length,org,coef);
  compose(length,dst,coef);

  max = 0.0;
  for (i = 0; i < length; i++) {
    double d = fabs(org[i]-dst[i]);
    if (max < d)
      max = d;
    s->data[0][i] = dst[i]+0.5;
  }
  fprintf(stderr,"Maximum difference: %g\n",max);

  free(org);
  free(coef);
}
 

int main(int argc, char *argv[])
{
  char *infile, *outfile;
  int ret,i;
  SoundData *s;

  i = 1;
  if (i+1 >= argc) {
    fprintf(stderr,"usage: %s infile outfile\n",argv[0]);
    exit(1);
  }

  infile = argv[i];
  outfile = argv[i+1];

  if ((s = readWavFile(infile,&ret)) == NULL) {
    fprintf(stderr,"%s : %s\n",infile,SndStrError(ret));
    exit(1);
  }
  process(s);
  writeWavFile(s,outfile);
  return 0;
}
