/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2" } */

#pragma omp declare simd notinbranch
extern double tan (double);
#pragma omp declare simd inbranch
extern double log (double);
void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = tan (x[i]) + log (x[i]);
}
/* { dg-final { scan-assembler-not "_ZGV" } } */
