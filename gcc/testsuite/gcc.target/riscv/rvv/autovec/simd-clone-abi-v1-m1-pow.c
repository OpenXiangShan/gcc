/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2" } */

#pragma omp declare simd notinbranch
extern double pow (double, double)
  __attribute__((riscv_simd_math_abi_v1(0x33)));

void
apply (double *restrict y, const double *restrict x,
       const double *restrict z, unsigned long n)
{
#pragma omp simd simdlen(2)
  for (unsigned long i = 0; i < n; ++i)
    y[i] = pow (x[i], z[i]);
}

/* The m1 clone takes its two data arguments in v8 and v9; m2 would start
   the second argument in v10.  */
/* { dg-final { scan-assembler "vl1re64\\.v[ \t]+v8," } } */
/* { dg-final { scan-assembler "vl1re64\\.v[ \t]+v9," } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV1N2vv_pow" } } */
/* { dg-final { scan-assembler "\\.variant_cc[ \t]+_ZGV1N2vv_pow" } } */
/* { dg-final { scan-assembler-not "_ZGV2" } } */
/* { dg-final { scan-assembler-not "_ZGV[12]M" } } */
