/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=scalable -mrvv-max-lmul=m2" } */
#pragma omp declare simd notinbranch
extern double exp (double);
#pragma omp declare simd notinbranch
extern double sin (double);
#pragma omp declare simd notinbranch
extern double cos (double);
#pragma omp declare simd notinbranch
extern double pow (double, double);
#pragma omp declare simd notinbranch
extern float logf (float);
#define FN(f) void f##loop (double *restrict o, const double *restrict a, unsigned long n) { for (unsigned long i=0;i<n;i++) o[i]=f(a[i]); }
FN(exp) FN(sin) FN(cos)
void powloop (double *restrict o, const double *restrict a, const double *restrict b, unsigned long n) { for (unsigned long i=0;i<n;i++) o[i]=pow(a[i],b[i]); }
void logfloop (float *restrict o, const float *restrict a, unsigned long n) { for (unsigned long i=0;i<n;i++) o[i]=logf(a[i]); }
/* { dg-final { scan-assembler "call[ \t]+_ZGV2Nxv_exp" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV2Nxv_sin" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV2Nxv_cos" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV2Nxvv_pow" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV2Nxv_logf" } } */
/* { dg-final { scan-assembler-not "_ZGV2M" } } */
