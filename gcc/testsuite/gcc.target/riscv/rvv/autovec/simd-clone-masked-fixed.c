/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fno-builtin-log -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2" } */
#ifndef __riscv_simd_math_log_m2_mask
#error missing masked fixed capability
#endif
#ifdef __riscv_simd_math_log_m2_mask_vla
#error scalable capability in fixed mode
#endif
#pragma omp declare simd inbranch
extern double log (double) __attribute__((const));
void apply (unsigned long n, const int *restrict enabled,
            const double *restrict x, double *restrict y)
{
  for (unsigned long i=0; i<n; ++i)
    if (enabled[i]) y[i] = log (x[i]);
}
/* { dg-final { scan-assembler "\\.variant_cc[ \t]+_ZGV2M4v_log" } } */
/* { dg-final { scan-assembler "vmv1r\\.v[ \t]+v0," } } */
/* { dg-final { scan-assembler "vle64\\.v[ \t]+v8," } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV2M4v_log" } } */
/* { dg-final { scan-assembler "call[ \t]+log" } } */
