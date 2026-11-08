/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -fdump-ipa-simdclone" } */

/* An explicit mask may request variants a function does not provide.  The
   target must intersect it with the entry's supported mask instead of
   creating an unsupported masked ABI.  Only log has masked kernels.  */
#pragma omp declare simd notinbranch
extern double exp (double) __attribute__((riscv_simd_math_abi_v1(0xff)));
#pragma omp declare simd notinbranch
extern double pow (double, double)
  __attribute__((riscv_simd_math_abi_v1(0xff)));
#pragma omp declare simd notinbranch
extern float logf (float) __attribute__((riscv_simd_math_abi_v1(0xff)));

#define U(f) \
  void f##loop (double *restrict y, const double *restrict x, unsigned long n) \
  { for (unsigned long i = 0; i < n; ++i) y[i] = f (x[i]); }
U (exp)
void
powloop (double *restrict y, const double *restrict x,
	 const double *restrict z, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = pow (x[i], z[i]);
}
void
logfloop (float *restrict y, const float *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = logf (x[i]);
}

/* { dg-final { scan-ipa-dump "_ZGV1N2v_exp" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2N4v_exp" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV1N2vv_pow" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2N4vv_pow" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV1N4v_logf" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2N8v_logf" "simdclone" } } */
/* { dg-final { scan-assembler-not "_ZGV[12]M" } } */
