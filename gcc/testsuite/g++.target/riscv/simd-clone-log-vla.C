/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=scalable -mrvv-max-lmul=m2" } */

#ifndef __riscv_simd_math_log_m2_vla
#error missing scalable compiler capability
#endif
extern "C" {
#pragma omp declare simd notinbranch
extern double log (double);
void
apply (double *__restrict y, const double *__restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}
}
/* { dg-final { scan-assembler "\\.variant_cc[ \t]+_ZGV2Nxv_log" } } */
/* { dg-final { scan-assembler "vl2re64\\.v[ \t]+v8," } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV2Nxv_log" } } */
/* { dg-final { scan-assembler "vs2r\\.v[ \t]+v8," } } */
/* { dg-final { scan-assembler-not "_ZGV2Mxv_log" } } */
