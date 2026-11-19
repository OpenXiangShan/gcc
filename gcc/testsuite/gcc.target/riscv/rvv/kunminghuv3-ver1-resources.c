/* { dg-do compile { target rv64 } } */
/* { dg-options "-O2 -march=rva23u64 -mabi=lp64d -mtune=xiangshan-kunminghuv3-ver1 -fdump-rtl-sched2-details" } */

#include <riscv_vector.h>

void
mix_m8 (float *out0, float *out1, float *out2, vfloat32m8_t a,
	vfloat32m8_t b, size_t vl)
{
  vfloat32m8_t sum = __riscv_vfadd_vv_f32m8 (a, b, vl);
  vfloat32m8_t prod = __riscv_vfmul_vv_f32m8 (a, b, vl);
  vfloat32m8_t acc = __riscv_vfmacc_vv_f32m8 (a, a, b, vl);
  __riscv_vse32_v_f32m8 (out0, sum, vl);
  __riscv_vse32_v_f32m8 (out1, prod, vl);
  __riscv_vse32_v_f32m8 (out2, acc, vl);
}

vint32m1_t
convert_m1 (vfloat32m1_t a, size_t vl)
{
  return __riscv_vfcvt_x_f_v_i32m1 (a, vl);
}

vuint32m2_t
narrow_m2 (vuint64m4_t a, size_t shift, size_t vl)
{
  return __riscv_vnsrl_wx_u32m2 (a, shift, vl);
}

/* EEW 16 for the index, EEW 32 and LMUL 2 for each data field.  */
vuint32m2_t
segment_e32m2 (const uint32_t *base, vuint16m1_t index, vuint32m2_t b,
	       size_t vl)
{
  vuint32m2x2_t pair = __riscv_vluxseg2ei16_v_u32m2x2 (base, index, vl);
  vuint32m2_t first = __riscv_vget_v_u32m2x2_u32m2 (pair, 0);
  return __riscv_vadd_vv_u32m2 (first, b, vl);
}

/* { dg-final { scan-rtl-dump {:khv3_s1_8} "sched2" } } */
/* { dg-final { scan-rtl-dump {:khv3_s2_8} "sched2" } } */
/* { dg-final { scan-rtl-dump {:khv3_s3_8} "sched2" } } */
/* { dg-final { scan-rtl-dump {khv3_vfcvt\+khv3_i0\+\(nothing,khv3_o0\)} "sched2" } } */
/* { dg-final { scan-rtl-dump {:khv3_n_nshift_8} "sched2" } } */
/* { dg-final { scan-assembler {vluxseg2ei16\.v} } } */
