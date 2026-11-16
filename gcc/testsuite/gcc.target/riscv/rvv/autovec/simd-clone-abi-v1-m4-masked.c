/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=scalable -mrvv-max-lmul=m4 -fdump-ipa-simdclone" } */

/* A masked LMUL4 clone uses the native LMUL4 data type and the matching
   vbool16_t mask type, exactly like the LMUL1/LMUL2 masked entries.  */
#pragma omp declare simd inbranch
extern double log (double) __attribute__((const, riscv_simd_math_abi_v1(0xffff)));

void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    if (x[i] > 0.0)
      y[i] = log (x[i]);
}

/* { dg-final { scan-ipa-dump "_ZGV4Mxv_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump-not "_ZGV8Mxv_log" "simdclone" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV4Mxv_log" } } */
/* { dg-final { scan-assembler "\\.variant_cc[ \t]+_ZGV4Mxv_log" } } */
/* { dg-final { scan-assembler-not "_ZGV8" } } */
