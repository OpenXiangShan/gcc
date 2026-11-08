/* { dg-do link { target rv64 } } */
/* { dg-require-effective-target lto } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -flto -flto-partition=one -save-temps -lm" } */
/* { dg-additional-sources "simd-clone-abi-v1-lto-absent-b.c" } */

/* Fixture declarations only; no math.h include, so the result is independent
   of the real-header versus fixture-declaration classification.  The checks
   scan the post-LTO LTRANS assembly, not the pre-LTO object.  */

/* A declaration without the ABI attribute contributes the legacy m2 mask,
   not the all-zero mask and not an inherited m1 mask.  The result must be
   the legacy-compatible 0x30 intersection, so only m2 is valid.  */

/* Link witness only: this vector_size stub is not a native RVV ABI or
   runtime proof; noinline keeps the m2 call visible in the LTRANS assembly.  */
typedef double v4df __attribute__((vector_size (32)));
__attribute__((used, noinline, noclone)) v4df
_ZGV2N4v_log (v4df x)
{
  return x;
}

extern double log (double)
  __attribute__((riscv_simd_math_abi_v1(0x33)));
#pragma omp declare simd notinbranch
extern double log (double);

__attribute__((noinline, noclone))
double
apply_a (double *restrict y, const double *restrict x, unsigned long n)
{
#pragma omp simd simdlen(2)
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
  return y[0];
}

extern double apply_plain (double *, const double *, unsigned long);

int
main (void)
{
  double x[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  double y[8] = {0};
  apply_a (y, x, 8);
  apply_plain (y, x, 8);
  return 0;
}

/* { dg-final { scan-lto-assembler "call[ \t]+_ZGV2N4v_log" } } */
/* { dg-final { scan-lto-assembler-not "_ZGV1N2v_log" } } */
