#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "wavio.h"

double amplitude1 = 1.0;
double amplitude2 = 1.0;

void do_normalize(SoundData *s1, SoundData *s2)
{
  int x1,x2,max1, max2;
  int i;

  max1 = max2 = 0;
  
  for (i = 0; i < s1->size && i < s2->size; i++) {
    x1 = abs(s1->data[0][i]);
    x2 = abs(s2->data[0][i]);
    if (x1 > max1) max1 = x1;
    if (x2 > max2) max2 = x2;
  }
  amplitude1 = 1.0/max1;
  amplitude2 = 1.0/max2;
}

int main(int argc, char *argv[])
{
  SoundData *s1, *s2;
  double p1,p2,e;
  int i,n,ret;
  int normalize = 0;

  for (i = 1; i < argc; i++) {
    if (!strcmp(argv[i],"-norm")) {
      normalize = 1;
    }
    else
      break;
  }
  if (i+1 >= argc) {
    fprintf(stderr,"usage: calc-error [-norm] file1 file2\n");
    exit(1);
  }
  if ((s1 = readWavFile(argv[i],&ret)) == NULL) {
    fprintf(stderr,"%s: %s\n",argv[i],SndStrError(ret));
    exit(1);
  }
  if ((s2 = readWavFile(argv[i+1],&ret)) == NULL) {
    fprintf(stderr,"%s: %s\n",argv[i+1],SndStrError(ret));
    exit(1);
  }
  if (normalize)
    do_normalize(s1,s2);

  n = 0;
  p1 = p2 = e = 0.0;
  for (i = 0; i < s1->size && i < s2->size; i++) {
    double d,x1,x2;
    n++;
    x1 = amplitude1*s1->data[0][i];
    x2 = amplitude2*s2->data[0][i];
    p1 += x1*x1;
    p2 += x2*x2;
    d = x1-x2;
    e += d*d;
  }
  printf("Sample count            :  %d\n",n);
  printf("Average power of file 1 :  %g\n",p1/n);
  printf("Average power of file 2 :  %g\n",p2/n);
  printf("Average error           :  %g\n",e/n);
  printf("Signal to noise ratio   :  %g dB (with respect to file 1)\n",
	 10*log(p1/e)/log(10));
  return 0;
}
