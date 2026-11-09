/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -fdump-ipa-simdclone" } */

#pragma omp declare simd inbranch
extern double atan2 (double, double)
  __attribute__((const, riscv_simd_math_abi_v1(0xff)));

void
apply (unsigned long n, const int *restrict enabled,
       const double *restrict x, const double *restrict z,
       double *restrict y)
{
  for (unsigned long i = 0; i < n; ++i)
    if (enabled[i])
      y[i] = atan2 (x[i], z[i]);
}

/* { dg-final { scan-ipa-dump "_ZGV1M2vv_atan2" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2M4vv_atan2" "simdclone" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV[12]M[24]vv_atan2" } } */
