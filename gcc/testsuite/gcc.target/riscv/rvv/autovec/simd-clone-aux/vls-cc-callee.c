typedef double v4df __attribute__ ((vector_size (32)));

v4df __attribute__ ((riscv_vls_cc (128), noinline))
vls_cc_probe (v4df x)
{
  return x + (v4df) { 3.0, 3.0, 3.0, 3.0 };
}
