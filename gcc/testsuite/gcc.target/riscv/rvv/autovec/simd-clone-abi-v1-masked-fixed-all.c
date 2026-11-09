/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fno-builtin-sin -fno-builtin-cos -fno-builtin-pow -fno-builtin-logf -fno-builtin-expf -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -fdump-ipa-simdclone" } */

/* The 2.45 runtime exports the masked m1 and m2 fixed entries for every
   kernel it provides.  An explicit full mask must therefore produce both
   masked fixed clones for the five newly covered functions.  */
#ifndef __riscv_simd_math_sin_m2_mask
#error missing masked fixed sin capability
#endif
#ifndef __riscv_simd_math_cos_m2_mask
#error missing masked fixed cos capability
#endif
#ifndef __riscv_simd_math_pow_m2_mask
#error missing masked fixed pow capability
#endif
#ifndef __riscv_simd_math_logf_m2_mask
#error missing masked fixed logf capability
#endif
#ifndef __riscv_simd_math_expf_m2_mask
#error missing masked fixed expf capability
#endif
#ifdef __riscv_simd_math_sin_m2_mask_vla
#error scalable capability in fixed mode
#endif

#pragma omp declare simd inbranch
extern double sin (double)
  __attribute__((const, riscv_simd_math_abi_v1(0xff)));
#pragma omp declare simd inbranch
extern double cos (double)
  __attribute__((const, riscv_simd_math_abi_v1(0xff)));
#pragma omp declare simd inbranch
extern double pow (double, double)
  __attribute__((const, riscv_simd_math_abi_v1(0xff)));
#pragma omp declare simd inbranch
extern float logf (float)
  __attribute__((const, riscv_simd_math_abi_v1(0xff)));
#pragma omp declare simd inbranch
extern float expf (float)
  __attribute__((const, riscv_simd_math_abi_v1(0xff)));

void
apply (unsigned long n, const int *restrict enabled,
       const double *restrict x, const double *restrict z,
       double *restrict y,
       const float *restrict xf, float *restrict yf)
{
  for (unsigned long i = 0; i < n; ++i)
    if (enabled[i])
      y[i] = sin (x[i]);
  for (unsigned long i = 0; i < n; ++i)
    if (enabled[i])
      y[i] = cos (x[i]);
  for (unsigned long i = 0; i < n; ++i)
    if (enabled[i])
      y[i] = pow (x[i], z[i]);
  for (unsigned long i = 0; i < n; ++i)
    if (enabled[i])
      yf[i] = logf (xf[i]);
  for (unsigned long i = 0; i < n; ++i)
    if (enabled[i])
      yf[i] = expf (xf[i]);
}

/* { dg-final { scan-ipa-dump "_ZGV1M2v_sin" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2M4v_sin" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV1M2v_cos" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2M4v_cos" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV1M2vv_pow" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2M4vv_pow" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV1M4v_logf" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2M8v_logf" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV1M4v_expf" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2M8v_expf" "simdclone" } } */
