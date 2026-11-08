/* { dg-do link { target rv64 } } */
/* { dg-require-effective-target lto } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -flto -flto-partition=one -save-temps -lm" } */
/* { dg-additional-sources "simd-clone-abi-v1-lto-zero-b.c" } */

/* Fixture declarations only; no math.h include, so the result is independent
   of the real-header versus fixture-declaration classification.  The check
   scans post-LTO LTRANS assembly.  */

/* An explicit 0x00 in the second TU must absorb the first TU's 0x33.
   No SIMD clone may be called or declared.  */

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

extern double apply_zero (double *, const double *, unsigned long);

int
main (void)
{
  double x[8] = {1, 2, 3, 4, 5, 6, 7, 8};
  double y[8] = {0};
  apply_a (y, x, 8);
  apply_zero (y, x, 8);
  return 0;
}

/* { dg-final { scan-lto-assembler-not "_ZGV" } } */
