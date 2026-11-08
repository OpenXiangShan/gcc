/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gc_zve32f_zvl128b -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2" } */

/* An explicit mask may expose float variants even without vector FP64.  */
#pragma omp declare simd notinbranch
extern float logf (float) __attribute__((riscv_simd_math_abi_v1(0x33)));

void
apply (float *restrict y, const float *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = logf (x[i]);
}

/* { dg-final { scan-assembler "call[ \t]+_ZGV1N4v_logf" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV2N8v_logf" } } */
