/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=scalable -mrvv-max-lmul=m1 -fdump-ipa-simdclone" } */

#pragma omp declare simd notinbranch
extern double log (double) __attribute__((riscv_simd_math_abi_v1(0xf3)));

void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}

/* Both scalable clones are generated; -mrvv-max-lmul=m1 makes the vectorizer
   select the m1 one.  */
/* { dg-final { scan-ipa-dump "_ZGV1Nxv_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2Nxv_log" "simdclone" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV1Nxv_log" } } */
/* { dg-final { scan-assembler "\\.variant_cc[ \t]+_ZGV1Nxv_log" } } */
/* { dg-final { scan-assembler-not "_ZGV[12]M" } } */
