/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -fdump-ipa-simdclone" } */

#pragma omp declare simd notinbranch
extern double atan2 (double, double)
  __attribute__((riscv_simd_math_abi_v1(0xff)));

void
apply (double *restrict y, const double *restrict x,
       const double *restrict z, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = atan2 (x[i], z[i]);
}

/* { dg-final { scan-ipa-dump "_ZGV1N2vv_atan2" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2N4vv_atan2" "simdclone" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV[12]N[24]vv_atan2" } } */
/* { dg-final { scan-assembler-not "_ZGV[12]M" } } */
