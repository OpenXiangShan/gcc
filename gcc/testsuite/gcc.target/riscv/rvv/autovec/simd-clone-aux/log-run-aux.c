/* Auxiliary implementations for simd-clone-log-run.c.  */
#include <riscv_vector.h>

/* The deliberately simple function checks argument and return lanes rather
   than the mathematical accuracy of the real library implementation.  */
vfloat64m2_t __attribute__((noinline))
_ZGV2N4v_log (vfloat64m2_t x)
{
  return __riscv_vfadd_vf_f64m2 (x, 10.0, 4);
}

double __attribute__((noinline))
log (double x)
{
  return x + 10.0;
}
