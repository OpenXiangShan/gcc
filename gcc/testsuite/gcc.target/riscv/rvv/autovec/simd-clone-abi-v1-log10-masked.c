/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -fdump-ipa-simdclone" } */

#pragma omp declare simd inbranch
extern double log10 (double) __attribute__((const, riscv_simd_math_abi_v1(0xff)));

void
apply (unsigned long n, const int *restrict enabled,
       const double *restrict x, double *restrict y)
{
  for (unsigned long i = 0; i < n; ++i)
    if (enabled[i])
      y[i] = log10 (x[i]);
}

/* { dg-final { scan-ipa-dump "_ZGV1M2v_log10" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2M4v_log10" "simdclone" } } */
/* { dg-final { scan-assembler "call[ 	]+_ZGV[12]M[24]v_log10" } } */
