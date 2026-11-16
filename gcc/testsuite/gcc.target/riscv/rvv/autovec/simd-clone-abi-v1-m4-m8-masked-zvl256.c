/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv_zvl256b -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m8 -fdump-ipa-simdclone" } */

/* Fixed-lane masked variants keep their ABI lane count only at the 128-bit
   baseline.  With a 256-bit minimum VLEN the LMUL4 and LMUL8 unmasked fixed
   clones still exist, but the fixed masked ones must not be proposed.  */
#pragma omp declare simd notinbranch
extern double log (double) __attribute__((riscv_simd_math_abi_v1(0xffff)));
#pragma omp declare simd inbranch
extern double exp (double) __attribute__((const, riscv_simd_math_abi_v1(0xffff)));

void
ulog (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}

void
mexp (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    if (x[i] > 0.0)
      y[i] = exp (x[i]);
}

/* { dg-final { scan-ipa-dump "_ZGV4N8v_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV8N16v_log" "simdclone" } } */
/* { dg-final { scan-ipa-dump-not "_ZGV[1248]M[0-9]*v_" "simdclone" } } */
/* { dg-final { scan-assembler-not "_ZGV[48]M" } } */
