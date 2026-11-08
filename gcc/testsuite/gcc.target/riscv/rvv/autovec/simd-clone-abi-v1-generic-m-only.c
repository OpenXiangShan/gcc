/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fno-builtin-log -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=scalable -mrvv-max-lmul=m2 -fdump-ipa-simdclone" } */

/* A bare simd clause explores both branches.  Here the notinbranch mask is
   empty while the inbranch mask has two scalable variants, so the generic
   pass must not drop the second branch when the first has no candidates.  */
#pragma omp declare simd
extern double log (double) __attribute__((riscv_simd_math_abi_v1(0xcc)));

void
apply (unsigned long n, const int *restrict enabled,
       const double *restrict x, double *restrict y)
{
  for (unsigned long i = 0; i < n; ++i)
    if (enabled[i])
      y[i] = log (x[i]);
}

/* { dg-final { scan-ipa-dump "_ZGV1Mxv_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2Mxv_log" "simdclone" } } */
/* { dg-final { scan-assembler-not "_ZGV[12]N" } } */
