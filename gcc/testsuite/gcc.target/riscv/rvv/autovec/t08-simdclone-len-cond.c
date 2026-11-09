/* Conditional length-controlled SIMD clone in an SLP call node.
   Analysis may select the length clone while partial vectors are still
   permitted; if the loop later operates on full vectors, transform must
   use the equivalent non-length masked clone rather than failing in the
   SLP scheduler.  */
/* { dg-do compile } */
/* { dg-require-effective-target riscv_v } */
/* { dg-options "-O3 -ffast-math -fvect-cost-model=unlimited -mrvv-max-lmul=m1" } */
/* { dg-final { scan-assembler "_ZGV1Mxv_log_len" } } */

#include <math.h>

extern double log (double) __attribute__ ((riscv_simd_math_abi_len));

void
f (double *__restrict y, const double *__restrict x, int n)
{
  for (int i = 0; i < n; i++)
    if (x[i] > 0.75)
      y[i] = log (x[i]);
}
