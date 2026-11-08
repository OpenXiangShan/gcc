/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -fdump-ipa-simdclone" } */

/* Both fixed variants are generated; without an explicit simdlen the
   vectorizer may select either one for the main loop.  */
#pragma omp declare simd notinbranch
extern double log (double) __attribute__((riscv_simd_math_abi_v1(0xf3)));

void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}

/* { dg-final { scan-ipa-dump "_ZGV1N2v_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2N4v_log" "simdclone" } } */
/* { dg-final { scan-assembler-not "_ZGV[12]M" } } */
