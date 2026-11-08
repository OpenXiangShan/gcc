/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2" } */

/* An explicit zero mask means no variants, even though an absent attribute
   would select the legacy m2 mask.  */
#pragma omp declare simd notinbranch
extern double log (double) __attribute__((riscv_simd_math_abi_v1(0)));

void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}

/* { dg-final { scan-assembler-not "_ZGV" } } */
/* { dg-final { scan-assembler "call[ \t]+log" } } */
