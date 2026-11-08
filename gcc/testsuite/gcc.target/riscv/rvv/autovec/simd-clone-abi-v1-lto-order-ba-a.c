/* { dg-do compile { target rv64 } } */
/* { dg-require-effective-target lto } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -flto -flto-partition=one -save-temps -lm" } */

extern double log (double)
  __attribute__((riscv_simd_math_abi_v1(0x33)));
#pragma omp declare simd notinbranch
extern double log (double);

__attribute__((noinline, noclone))
double
apply_a (double *restrict y, const double *restrict x, unsigned long n)
{
#pragma omp simd simdlen(2)
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
  return y[0];
}
