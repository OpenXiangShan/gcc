/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=scalable -mrvv-max-lmul=m4 -fdump-ipa-simdclone" } */

/* The 16-bit mask capability macro must be visible so that a library header
   can emit the LMUL4 and LMUL8 nibbles.  */
#ifndef __riscv_simd_math_abi_v1_lmul8
#error missing 16-bit libmvec variant mask capability
#endif

#pragma omp declare simd notinbranch
extern double log (double) __attribute__((riscv_simd_math_abi_v1(0xffff)));

void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}

/* -mrvv-max-lmul=m4 proposes the LMUL1, LMUL2 and LMUL4 scalable clones and
   the vectorizer selects the LMUL4 one.  No LMUL8 clone may be created.  */
/* { dg-final { scan-ipa-dump "_ZGV1Nxv_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2Nxv_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV4Nxv_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump-not "_ZGV8Nxv_log" "simdclone" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV4Nxv_log" } } */
/* { dg-final { scan-assembler "\\.variant_cc[ \t]+_ZGV4Nxv_log" } } */
/* { dg-final { scan-assembler-not "_ZGV8" } } */
