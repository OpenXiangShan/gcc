/* Test LTO SIMD-clone formal types after RVV builtin type registration.
   The callee is a synthetic identity function, not a libmvec implementation.
   In particular, check that an actual masked call was made rather than
   accepting a scalar fallback.  */
/* { dg-lto-do run } */
/* { dg-require-effective-target rv64 } */
/* { dg-require-effective-target riscv_v_ok } */
/* { dg-lto-options { { -O3 -ffast-math -fno-builtin-log -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-max-lmul=m2 -mrvv-vector-bits=scalable -flto } { -O3 -ffast-math -fno-builtin-log -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-max-lmul=m2 -mrvv-vector-bits=zvl -DTEST_FIXED -flto } } } */

#pragma omp declare simd inbranch
extern double log (double) __attribute__ ((const));
extern volatile unsigned long masked_calls, masked_lanes;

__attribute__ ((noipa)) void
apply (double *restrict dst, const double *restrict src,
       const int *restrict enabled, int n)
{
#pragma omp simd
  for (int i = 0; i < n; ++i)
    if (enabled[i])
      dst[i] = log (src[i]);
}

int
main (void)
{
#ifdef TEST_FIXED
  /* Read the hardware VLEN: vsetvlmax can be constant-folded under zvl.  */
  unsigned long vlenb;
  __asm__ volatile ("csrr %0, vlenb" : "=r" (vlenb));
  if (vlenb != 16)
    return 0;
#endif
  /* Even at the architectural maximum VLEN of 65536 bits, 2048 doubles
     is an exact multiple of e64,m2 VLMAX.  */
  double src[2048], dst[2048];
  int enabled[2048];
  for (int i = 0; i < 2048; ++i)
    {
      src[i] = i + 1.0;
      dst[i] = -1.0;
      enabled[i] = i & 1;
    }
  apply (dst, src, enabled, 2048);
  if (!masked_calls || masked_lanes != 1024)
    __builtin_abort ();
  for (int i = 0; i < 2048; ++i)
    if (dst[i] != (enabled[i] ? src[i] : -1.0))
      __builtin_abort ();
  return 0;
}
