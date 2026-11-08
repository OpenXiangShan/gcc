/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2" } */

#ifndef __riscv_simd_math_abi_v1
#error missing ABI v1 syntax capability
#endif
#if __riscv_simd_math_abi_v1 != 1
#error unexpected ABI v1 capability value
#endif

#pragma omp declare simd notinbranch
extern double log (double) __attribute__((riscv_simd_math_abi_v1(0xf3)));

void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
#pragma omp simd simdlen(2)
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}

/* { dg-final { scan-assembler "vl1re64\\.v[ \t]+v8," } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV1N2v_log" } } */
/* { dg-final { scan-assembler "vs1r\\.v[ \t]+v8," } } */
/* { dg-final { scan-assembler "\\.variant_cc[ \t]+_ZGV1N2v_log" } } */
/* { dg-final { scan-assembler-not "_ZGV2" } } */
/* { dg-final { scan-assembler-not "_ZGV[12]M" } } */
