/* { dg-do compile { target rv64 } } */
/* { dg-require-effective-target lto } */
/* { dg-require-effective-target riscv_v } */
/* { dg-options "-O2 -ffast-math -fopenmp-simd -fno-builtin-log -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -flto -flto-partition=one -save-temps -lm" } */

/* Declare log explicitly with an inbranch SIMD clone instead of relying on
   the installed <math.h> to provide the masked clone declaration, so that
   this test is independent of the libc version.  */

#pragma omp declare simd inbranch
extern double log (double)
  __attribute__((const, riscv_simd_math_abi_v1 (0xff)));

void
worker (double *y, const double *x)
{
#pragma omp simd
  for (int i = 0; i < 64; ++i)
    if (x[i] > 0.0)
      y[i] = log (x[i]);
    else
      y[i] = -1.0;
}
