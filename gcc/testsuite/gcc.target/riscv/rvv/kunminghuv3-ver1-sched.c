/* { dg-do compile { target rv64 } } */
/* { dg-options "-O2 -march=rva23u64 -mabi=lp64d -mtune=xiangshan-kunminghuv3-ver1 --param=gpr2vr-cost=0 --param=fpr2vr-cost=0 -fdump-rtl-sched2-details" } */

#include <riscv_vector.h>

/* The two add kernels have the same number of operations and stores, but
   only the second has a chain of true dependencies between the adds.  */
void
independent_e32 (float *__restrict out0, float *__restrict out1,
		 float *__restrict out2, vfloat32m1_t a, vfloat32m1_t b,
		 vfloat32m1_t c, size_t vl)
{
  vfloat32m1_t x = __riscv_vfadd_vv_f32m1 (a, b, vl);
  vfloat32m1_t y = __riscv_vfadd_vv_f32m1 (b, c, vl);
  vfloat32m1_t z = __riscv_vfadd_vv_f32m1 (a, c, vl);
  __riscv_vse32_v_f32m1 (out0, x, vl);
  __riscv_vse32_v_f32m1 (out1, y, vl);
  __riscv_vse32_v_f32m1 (out2, z, vl);
}

void
dependent_e32 (float *__restrict out0, float *__restrict out1,
	       float *__restrict out2, vfloat32m1_t a, vfloat32m1_t b,
	       vfloat32m1_t c, size_t vl)
{
  vfloat32m1_t x = __riscv_vfadd_vv_f32m1 (a, b, vl);
  vfloat32m1_t y = __riscv_vfadd_vv_f32m1 (x, c, vl);
  vfloat32m1_t z = __riscv_vfadd_vv_f32m1 (y, b, vl);
  __riscv_vse32_v_f32m1 (out0, x, vl);
  __riscv_vse32_v_f32m1 (out1, y, vl);
  __riscv_vse32_v_f32m1 (out2, z, vl);
}

/* Independent producers with different EX result slots.  */
vfloat32m1_t
mixed_e32 (vfloat32m1_t a, vfloat32m1_t b, vfloat32m1_t c,
	   vfloat32m1_t d, vfloat32m1_t e, size_t vl)
{
  vfloat32m1_t sum = __riscv_vfadd_vv_f32m1 (a, b, vl);
  vfloat32m1_t prod = __riscv_vfmul_vv_f32m1 (c, d, vl);
  vfloat32m1_t acc = __riscv_vfmacc_vv_f32m1 (e, a, c, vl);
  sum = __riscv_vfadd_vv_f32m1 (sum, prod, vl);
  return __riscv_vfadd_vv_f32m1 (sum, acc, vl);
}

void
load_use_e32 (int32_t *__restrict out, const int32_t *a, const int32_t *b,
	      size_t vl)
{
  vint32m1_t x = __riscv_vle32_v_i32m1 (a, vl);
  vint32m1_t y = __riscv_vle32_v_i32m1 (b, vl);
  vint32m1_t sum = __riscv_vadd_vv_i32m1 (x, y, vl);
  __riscv_vse32_v_i32m1 (out, sum, vl);
}

/* Non-unit-LMUL cases for the divider and permute fallback costs.  */
vint64m2_t
divide_e64m2 (vint64m2_t a, vint64m2_t b, vint64m2_t c, size_t vl)
{
  vint64m2_t quot = __riscv_vdiv_vv_i64m2 (a, b, vl);
  return __riscv_vadd_vv_i64m2 (quot, c, vl);
}

vuint32m4_t
gather_e32m4 (vuint32m4_t a, vuint32m4_t index, vuint32m4_t b, size_t vl)
{
  vuint32m4_t perm = __riscv_vrgather_vv_u32m4 (a, index, vl);
  return __riscv_vadd_vv_u32m4 (perm, b, vl);
}

/* Look for the DFA resource expressions, not particular issue cycles or
   instruction order.  */
/* { dg-final { scan-rtl-dump {:khv3_s1_1} "sched2" } } */
/* { dg-final { scan-rtl-dump {:khv3_s2_1} "sched2" } } */
/* { dg-final { scan-rtl-dump {:khv3_s3_1} "sched2" } } */
/* { dg-final { scan-rtl-dump {:khv3_vload} "sched2" } } */
/* { dg-final { scan-rtl-dump {khv3_idiv[*]16} "sched2" } } */
/* { dg-final { scan-rtl-dump {khv3_xv[01][*]13} "sched2" } } */
