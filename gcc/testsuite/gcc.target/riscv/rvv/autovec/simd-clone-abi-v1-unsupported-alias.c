/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2" } */

/* glibc's _FloatN math declarations can reuse a base SIMD declaration
   macro.  A valid mask on such an alias must be accepted silently and
   simply produce no clone; it must not make <math.h> fail or warn.  */
#pragma omp declare simd notinbranch
extern _Float64 logf64 (_Float64)
  __attribute__((riscv_simd_math_abi_v1(0xf3)));
#pragma omp declare simd notinbranch
extern _Float32 logf32 (_Float32)
  __attribute__((riscv_simd_math_abi_v1(0xf3)));

void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = (double) logf64 ((_Float64) x[i]);
}

void
applyf (float *restrict y, const float *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = (float) logf32 ((_Float32) x[i]);
}

/* { dg-final { scan-assembler-not "_ZGV" } } */
