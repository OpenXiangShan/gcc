/* { dg-do compile { target rv64 } } */
/* { dg-require-effective-target lto } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -flto -flto-partition=one -save-temps -lm" } */

#pragma omp declare simd notinbranch
extern double log (double);

__attribute__((noinline, noclone))
double
apply_plain (double *restrict y, const double *restrict x, unsigned long n)
{
#pragma omp simd simdlen(4)
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]) + 3.0;
  return y[0];
}
