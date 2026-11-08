/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fno-builtin-log -fopenmp-simd -march=rv64gcv_zvl256b -mabi=lp64d -mrvv-vector-bits=scalable -mrvv-max-lmul=m2" } */
#if defined (__riscv_simd_math_log_m2_mask) || defined (__riscv_simd_math_log_m2_mask_vla)
#error unexpected masked log capability
#endif
#pragma omp declare simd inbranch
extern double log (double) __attribute__((const));
void apply (unsigned long n, const int *restrict enabled,
            const double *restrict x, double *restrict y)
{
  for (unsigned long i=0; i<n; ++i)
    if (enabled[i]) y[i] = log (x[i]);
}
/* { dg-final { scan-assembler-not "_ZGV2M" } } */
