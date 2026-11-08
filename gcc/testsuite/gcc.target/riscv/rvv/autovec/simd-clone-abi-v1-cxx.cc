/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -std=c++17" } */

extern "C" double log (double)
  __attribute__((riscv_simd_math_abi_v1(0xf3)));

#pragma omp declare simd notinbranch
extern "C" double log (double);

void
apply (double *__restrict y, const double *__restrict x, unsigned long n)
{
#pragma omp simd simdlen(2)
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}

/* { dg-final { scan-assembler "call[ \t]+_ZGV1N2v_log" } } */
/* { dg-final { scan-assembler "\\.variant_cc[ \t]+_ZGV1N2v_log" } } */
