/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=scalable -fdump-ipa-simdclone" } */

/* No -mrvv-max-lmul option: the default maximum LMUL is 1, so a sixteen-bit
   mask must still produce exactly the LMUL1 and LMUL2 clones an eight-bit
   mask produces, and no LMUL4 or LMUL8 clone.  LMUL1 and LMUL2 are never
   gated by -mrvv-max-lmul, so the candidate list of the default
   configuration is unchanged by the wider mask.  */
#pragma omp declare simd notinbranch
extern double log (double) __attribute__((riscv_simd_math_abi_v1(0xffff)));

void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}

/* { dg-final { scan-ipa-dump "_ZGV1Nxv_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2Nxv_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump-not "_ZGV4" "simdclone" } } */
/* { dg-final { scan-ipa-dump-not "_ZGV8" "simdclone" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV1Nxv_log" } } */
/* { dg-final { scan-assembler-not "_ZGV[48]" } } */
