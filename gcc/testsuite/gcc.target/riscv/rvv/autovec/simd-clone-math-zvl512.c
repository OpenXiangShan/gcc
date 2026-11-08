/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv_zvl512b -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m1" } */
#pragma omp declare simd notinbranch
extern double pow (double, double) __attribute__((riscv_simd_math_abi_v1(51)));
void pow_loop (double *restrict y, const double *restrict x,
               const double *restrict z, int n)
{
  for (int i = 0; i < n; ++i) y[i] = pow (x[i], z[i]);
}
/* The fixed m2 symbol still consumes four lanes, not the eight lanes in
   a complete m1 register at this minimum VLEN.  */
/* { dg-final { scan-assembler "call[ \\t]+_ZGV2N4vv_pow" } } */
/* { dg-final { scan-assembler-not "_ZGV2Nxvv_pow" } } */
/* ABI slots must remain m2-aligned even though four active lanes fit in
   one physical register.  The second argument belongs in v10, not v9.  */
/* { dg-final { scan-assembler "vle64[.]v[ \\t]+v10," } } */
