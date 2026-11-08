/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fno-builtin-log -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2" } */

#pragma omp declare simd inbranch
extern double log (double)
  __attribute__((const, riscv_simd_math_abi_v1(0xff)));

void
apply (unsigned long n, const int *restrict enabled,
       const double *restrict x, double *restrict y)
{
#pragma omp simd simdlen(2)
  for (unsigned long i = 0; i < n; ++i)
    if (enabled[i])
      y[i] = log (x[i]);
}

/* { dg-final { scan-assembler "vmv1r\\.v[ \t]+v0," } } */
/* { dg-final { scan-assembler "vle64\\.v[ \t]+v8,.*,v0\\.t" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV1M2v_log" } } */
/* { dg-final { scan-assembler "\\.variant_cc[ \t]+_ZGV1M2v_log" } } */
/* { dg-final { scan-assembler-not "_ZGV2" } } */
