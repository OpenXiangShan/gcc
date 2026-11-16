/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O2 -march=rv64gcv -mabi=lp64d" } */

extern double log (double)
  __attribute__((riscv_simd_math_abi_v1(65536))); /* { dg-error "range 0 to 65535" } */
extern double exp (double)
  __attribute__((riscv_simd_math_abi_v1(-1))); /* { dg-error "range 0 to 65535" } */
