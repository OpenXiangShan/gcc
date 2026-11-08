/* Ordinary user VLS CC GNU vectors keep the existing indirect ABI.  The
   compiler's private SIMD-clone adaptation must not change it.  */
/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O2 -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl" } */

typedef double v4df __attribute__ ((vector_size (32)));
__attribute__ ((riscv_vls_cc (256), noinline)) v4df
ordinary_vls (v4df a, v4df b)
{
  return a + b;
}

/* { dg-final { scan-assembler "vl2re64\\.v[ \t]+v[0-9]+,0\\(a1\\)" } } */
/* { dg-final { scan-assembler "vl2re64\\.v[ \t]+v[0-9]+,0\\(a2\\)" } } */
/* { dg-final { scan-assembler "vs2r\\.v[ \t]+v[0-9]+,0\\(a0\\)" } } */
