/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fno-builtin-sin -fno-builtin-cos -fno-builtin-pow -fno-builtin-logf -fno-builtin-expf -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=scalable -mrvv-max-lmul=m1 -fdump-ipa-simdclone" } */

/* Scalable masked clones must exist for the five newly covered kernels;
   the fixed-lane capability macros must stay undefined in this mode.  */
#ifndef __riscv_simd_math_sin_m2_mask_vla
#error missing masked scalable sin capability
#endif
#ifndef __riscv_simd_math_cos_m2_mask_vla
#error missing masked scalable cos capability
#endif
#ifndef __riscv_simd_math_pow_m2_mask_vla
#error missing masked scalable pow capability
#endif
#ifndef __riscv_simd_math_logf_m2_mask_vla
#error missing masked scalable logf capability
#endif
#ifndef __riscv_simd_math_expf_m2_mask_vla
#error missing masked scalable expf capability
#endif
#ifdef __riscv_simd_math_sin_m2_mask
#error fixed capability in scalable mode
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

/* -mrvv-max-lmul=m1 makes the vectorizer select the m1 clones, but the
   clone pass still records the m2 candidates in the dump.  */
/* { dg-final { scan-ipa-dump "_ZGV1Mxv_sin" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2Mxv_sin" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV1Mxv_cos" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2Mxv_cos" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV1Mxvv_pow" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2Mxvv_pow" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV1Mxv_logf" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2Mxv_logf" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV1Mxv_expf" "simdclone" } } */
/* { dg-final { scan-ipa-dump "_ZGV2Mxv_expf" "simdclone" } } */
