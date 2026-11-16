/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=scalable -mrvv-max-lmul=m8 -fdump-ipa-simdclone" } */

#pragma omp declare simd notinbranch
extern double log (double) __attribute__((riscv_simd_math_abi_v1(0xffff)));

void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}

/* -mrvv-max-lmul=m8 additionally proposes the LMUL8 scalable clone and the
   vectorizer selects it.  */
/* { dg-final { scan-ipa-dump "_ZGV4Nxv_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV8Nxv_log" "simdclone" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV8Nxv_log" } } */
/* { dg-final { scan-assembler "\\.variant_cc[ \t]+_ZGV8Nxv_log" } } */
