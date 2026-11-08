/* Synthetic vector and scalar identity functions for rvv-log-masked_0.c.
   Keep the scalar definition in assembler so LTO sees an external scalar
   declaration and generates an external SIMD clone, as it does for libmvec.  */
#include <riscv_vector.h>

volatile unsigned long masked_calls, masked_lanes;

__attribute__ ((noipa)) vfloat64m2_t
_ZGV2Mxv_log (vbool32_t mask, vfloat64m2_t x)
{
  size_t vl = __riscv_vsetvlmax_e64m2 ();
  ++masked_calls;
  masked_lanes += __riscv_vcpop_m_b32 (mask, vl);
  return x;
}

__attribute__ ((noipa)) vfloat64m2_t
_ZGV2M4v_log (vbool32_t mask, vfloat64m2_t x)
{
  ++masked_calls;
  masked_lanes += __riscv_vcpop_m_b32 (mask, 4);
  return x;
}

__asm__ (".text\n"
         ".globl log\n"
         ".type log,@function\n"
         "log:\n"
         "ret\n"
         ".size log, .-log\n");
