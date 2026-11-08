/* { dg-do run { target { rv64 } } } */
/* { dg-require-effective-target riscv_v } */
/* { dg-options "-O2 -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=scalable" } */
#include <riscv_vector.h>
#include <stddef.h>

/* Ordinary public RVV ABI remains valid after the simd-clone lowering change.
   The callee must establish its own full VL regardless of incoming vtype/VL.  */
__attribute__ ((noinline, noipa)) vfloat64m2_t
rvv_abi_probe (vfloat64m2_t x)
{
  size_t vl = __riscv_vsetvlmax_e64m2 ();
  return __riscv_vfadd_vf_f64m2 (x, 3.0, vl);
}

int main (void)
{
  double in[128], out[128];
  size_t n = __riscv_vsetvlmax_e64m2 ();
  if (n > 128)
    return 0;
  for (size_t i = 0; i < n; ++i)
    in[i] = i + 0.25;
  vfloat64m2_t x = __riscv_vle64_v_f64m2 (in, n);
  __asm__ volatile ("vsetivli zero,0,e8,m1,ta,ma" ::: "memory");
  vfloat64m2_t y = rvv_abi_probe (x);
  __riscv_vse64_v_f64m2 (out, y, n);
  for (size_t i = 0; i < n; ++i)
    if (out[i] != in[i] + 3.0)
      __builtin_abort ();
  return 0;
}
