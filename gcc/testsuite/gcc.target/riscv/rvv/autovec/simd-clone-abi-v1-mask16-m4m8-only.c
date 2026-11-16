/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=scalable -mrvv-max-lmul=m8 -fdump-ipa-simdclone" } */

/* Mask 0x3300 selects only the unmasked LMUL4 and LMUL8 variants; the LMUL1
   and LMUL2 nibbles stay clear even though they are in the low eight bits.  */
#pragma omp declare simd notinbranch
extern double log (double) __attribute__((riscv_simd_math_abi_v1(0x3300)));

void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}

/* { dg-final { scan-ipa-dump "_ZGV4Nxv_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV8Nxv_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump-not "_ZGV1N" "simdclone" } } */
/* { dg-final { scan-ipa-dump-not "_ZGV2N" "simdclone" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV8Nxv_log" } } */
