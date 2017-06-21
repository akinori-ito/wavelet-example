#include <stdlib.h>
#include "wavelet.h"

/* coefficients for Daubechies' wavelet (N=4) */
static double daubechies4_p[] = {
  0.23037781330889,
  0.71484657055291,
  0.63088076792986,
  -0.02798376941686,
  -0.18703481171909,
  0.03084138183556,
  0.03288301166689,
  -0.01059740178507,
};

struct wavelet_coef Daubechies4 = {
  8, daubechies4_p
};

/* coefficients for Daubechies' wavelet (N=3) */
static double daubechies3_p[] = {
 3.326705529500826159985115891390056300129233992450683597084705e-01,
 8.068915093110925764944936040887134905192973949948236181650920e-01,
 4.598775021184915700951519421476167208081101774314923066433867e-01,
-1.350110200102545886963899066993744805622198452237811919756862e-01,
-8.544127388202666169281916918177331153619763898808662976351748e-02,
 3.522629188570953660274066471551002932775838791743161039893406e-02};

struct wavelet_coef Daubechies3 = {
  6, daubechies3_p
};
  

/* Daubechies' wavelet (N=2) */
static double daubechies2_p[] = {
  0.48296291314453,
  0.83651630373781,
  0.22414386804201,
  -0.12940952255126,
};

struct wavelet_coef Daubechies2 = {
  4, daubechies2_p
};


/* Haar */
static double haar_p[] = {
  0.7071067811865475,
  0.7071067811865475,
};

struct wavelet_coef Haar = {
  2, haar_p
};

static double *p,*q;
static int ncoef;

void prepare(struct wavelet_coef *coef)
{
  int i;
  int s = 1;
  ncoef = coef->ncoef;
  p = coef->coefs;
  q = (double*)malloc(sizeof(double)*ncoef);
  for (i = 0; i < ncoef; i++) {
    q[i] = s*p[ncoef-1-i];
    s *= -1;
  }
}

void decompose(int n, double *org, double *wavcoef)
{
  int i,k,ind;
  double *lo = wavcoef;
  double *hi = wavcoef+n/2;
  for (i = 0; i < n/2; i++) {
    hi[i] = lo[i] = 0;
    for (k = 0; k < ncoef; k++) {
      ind = (2*i+k)%n;
      lo[i] += p[k]*org[ind];
      hi[i] += q[k]*org[ind];
    }
  }
}

void compose(int n, double *dst, double *wavcoef)
{
  int i,k,ind;
  double *lo = wavcoef;
  double *hi = wavcoef+n/2;
  for (i = 0; i < n; i+=2) {
    dst[i] = dst[i+1] = 0.0;
    for (k = 0; k < ncoef/2; k++) {
      ind = i/2-k;
      if (ind < 0) ind += n/2;
      dst[i]   += p[2*k]*lo[ind]+q[2*k]*hi[ind];
      dst[i+1] += p[2*k+1]*lo[ind]+q[2*k+1]*hi[ind];
    }
  }
}
