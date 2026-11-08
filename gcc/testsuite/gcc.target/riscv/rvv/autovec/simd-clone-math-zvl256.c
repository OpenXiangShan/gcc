/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv_zvl256b -mabi=lp64d -mrvv-vector-bits=scalable -mrvv-max-lmul=m1" } */
#pragma omp declare simd notinbranch
extern double exp (double) __attribute__((riscv_simd_math_abi_v1(51)));
#pragma omp declare simd inbranch
extern double log (double) __attribute__((riscv_simd_math_abi_v1(255)));
void exp_loop (double *restrict y, const double *restrict x, int n)
{
  for (int i = 0; i < n; ++i) y[i] = exp (x[i]);
}
void log_loop (double *restrict y, const double *restrict x, int n)
{
  for (int i = 0; i < n; ++i) if (x[i] > 0) y[i] = log (x[i]);
}
/* { dg-final { scan-assembler "call[ \\t]+_ZGV1Nxv_exp" } } */
/* { dg-final { scan-assembler "call[ \\t]+_ZGV1Mxv_log" } } */
/* { dg-final { scan-assembler-not "_ZGV1N2v_exp" } } */
