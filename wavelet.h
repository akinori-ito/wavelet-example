struct wavelet_coef {
  int ncoef;
  double *coefs;
};

void prepare(struct wavelet_coef *coef);
void decompose(int n, double *org, double *wavcoef);
void compose(int n, double *dst, double *wavcoef);

extern struct wavelet_coef Daubechies4;
extern struct wavelet_coef Daubechies3;
extern struct wavelet_coef Daubechies2;
extern struct wavelet_coef Haar;

#define DAUBECHIES4 (&Daubechies4)
#define DAUBECHIES3 (&Daubechies3)
#define DAUBECHIES2 (&Daubechies2)
#define HAAR (&Haar)



