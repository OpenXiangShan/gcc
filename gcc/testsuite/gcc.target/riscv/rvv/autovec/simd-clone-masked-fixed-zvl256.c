/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fno-builtin-log -fopenmp-simd -march=rv64gcv_zvl256b -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2" } */
#if defined (__riscv_simd_math_log_m2_mask) || defined (__riscv_simd_math_log_m2_mask_vla)
#error unsupported wide fixed masked capability
#endif
#pragma omp declare simd inbranch
extern double log (double) __attribute__((const, riscv_simd_math_abi_v1(255)));
void apply (unsigned long n, const int *restrict enabled,
            const double *restrict x, double *restrict y)
{
  for (unsigned long i = 0; i < n; ++i)
    if (enabled[i]) y[i] = log (x[i]);
}
/* An explicit full library mask must not manufacture a native full-width
   masked prototype for a fixed four-lane symbol.  */
/* { dg-final { scan-assembler-not "_ZGV[12]M" } } */
