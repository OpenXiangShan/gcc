/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -fdump-ipa-simdclone" } */

#pragma omp declare simd notinbranch
extern double log10 (double) __attribute__((riscv_simd_math_abi_v1(0xff)));

void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log10 (x[i]);
}

/* { dg-final { scan-ipa-dump "_ZGV1N2v_log10" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2N4v_log10" "simdclone" } } */
/* { dg-final { scan-assembler "call[ 	]+_ZGV[12]N[24]v_log10" } } */
/* { dg-final { scan-assembler-not "_ZGV[12]M" } } */
