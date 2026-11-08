/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=scalable -mrvv-max-lmul=m2 -fdump-ipa-simdclone" } */
/* With no branch clause the generic IPA pass considers both variants.  The
   target must reject unsupported inbranch clones before signature adjustment. */
#pragma omp declare simd
extern double exp (double);
#pragma omp declare simd
extern double sin (double);
#pragma omp declare simd
extern double cos (double);
#pragma omp declare simd
extern double pow (double, double);
#pragma omp declare simd
extern float logf (float);
#define U(f) void f##loop (double *restrict o, const double *restrict x, unsigned long n) { for (unsigned long i=0;i<n;i++) o[i]=f(x[i]); }
U(exp) U(sin) U(cos)
void powloop (double *restrict o, const double *restrict x, const double *restrict y, unsigned long n) { for (unsigned long i=0;i<n;i++) o[i]=pow(x[i],y[i]); }
void logfloop (float *restrict o, const float *restrict x, unsigned long n) { for (unsigned long i=0;i<n;i++) o[i]=logf(x[i]); }
void conditional (double *restrict o, const double *restrict x, unsigned long n) { for (unsigned long i=0;i<n;i++) if (x[i]>1) o[i]=sin(x[i]); }
/* { dg-final { scan-ipa-dump "_ZGV2Nxv_exp" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2Nxvv_pow" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2Nxv_logf" "simdclone" } } */
/* { dg-final { scan-ipa-dump-not "_ZGV2M" "simdclone" } } */
/* { dg-final { scan-assembler-not "_ZGV2M" } } */
