/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv_zvl256b -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2" } */

#ifndef __riscv_simd_math_log_m2
#error missing wide fixed SIMD math capability marker
#endif
#pragma omp declare simd notinbranch
extern double log (double);
void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}
/* Wide fixed calls retain the public four-lane ABI.  */
/* { dg-final { scan-assembler "call[ \t]+_ZGV2N4v_log" } } */
/* { dg-final { scan-assembler-not "_ZGV2Nxv_log" } } */
