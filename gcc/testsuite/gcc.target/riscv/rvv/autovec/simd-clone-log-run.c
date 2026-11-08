/* { dg-do run { target { rv64 } } } */
/* { dg-require-effective-target riscv_v } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2" } */
/* { dg-additional-sources "simd-clone-aux/log-run-aux.c" } */

#pragma omp declare simd notinbranch
extern double log (double);

__attribute__((noinline)) void
apply (double *restrict y, const double *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = log (x[i]);
}

int
main (void)
{
  unsigned long vlenb;
  __asm__ volatile ("csrr %0, vlenb" : "=r" (vlenb));
  if (vlenb != 16)
    return 0;
  double x[37], y[37];
  for (unsigned n = 0; n <= 37; ++n)
    {
      for (unsigned i = 0; i < 37; ++i)
        x[i] = i + 0.25, y[i] = -1.0;
      apply (y, x, n);
      for (unsigned i = 0; i < 37; ++i)
        if (y[i] != (i < n ? x[i] + 10.0 : -1.0))
          __builtin_abort ();
    }
  return 0;
}
