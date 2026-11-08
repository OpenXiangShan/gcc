/* { dg-do run { target { rv64 } } } */
/* { dg-require-effective-target riscv_v } */
/* { dg-options "-O2 -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl" } */
/* { dg-additional-sources "simd-clone-aux/vls-cc-callee.c" } */

typedef double v4df __attribute__ ((vector_size (32)));
extern v4df vls_cc_probe (v4df) __attribute__ ((riscv_vls_cc (128)));

int
main (void)
{
  unsigned long vlenb;
  __asm__ volatile ("csrr %0, vlenb" : "=r" (vlenb));
  if (vlenb != 16)
    return 0;
  v4df in = { -5.0, 0.0, 8.0, 0.25 };
  v4df out = vls_cc_probe (in);
  v4df (__attribute__ ((riscv_vls_cc (128))) *volatile fn) (v4df)
    = vls_cc_probe;
  v4df indirect = fn (in);
  for (unsigned i = 0; i < 4; ++i)
    if (out[i] != in[i] + 3.0 || indirect[i] != out[i])
      __builtin_abort ();
  return 0;
}
