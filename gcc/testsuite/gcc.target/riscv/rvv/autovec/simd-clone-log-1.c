/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -fdump-tree-vect-details" } */

#ifndef __riscv_simd_math_log_m2
#error missing SIMD math capability marker
#endif
#pragma omp declare simd notinbranch
extern double log (double);

void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}

/* { dg-final { scan-assembler "\\.variant_cc[ \t]+_ZGV2N4v_log" } } */
/* { dg-final { scan-assembler "vl2re64\\.v[ \t]+v8," } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV2N4v_log" } } */
/* { dg-final { scan-assembler "vs2r\\.v[ \t]+v8," } } */
/* { dg-final { scan-assembler-not "_ZGV2M4v_log" } } */
/* { dg-final { scan-tree-dump "vectorizable_simd_clone_call" "vect" } } */
