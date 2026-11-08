/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gc -mabi=lp64d" } */

#ifdef __riscv_simd_math_log_m2
#error unexpected SIMD math capability marker
#endif
#pragma omp declare simd notinbranch
extern double log (double);
void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}
/* { dg-final { scan-assembler-not "_ZGV" } } */
/* { dg-final { scan-assembler "call[ \t]+log" } } */
