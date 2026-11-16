/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=scalable -mrvv-max-lmul=m8 -fdump-ipa-simdclone" } */

/* Single precision LMUL8 entries keep 8 vectors of floats (32 lanes at the
   128-bit baseline) and a two-argument kernel keeps its vv mangling.  */
#pragma omp declare simd notinbranch
extern float logf (float) __attribute__((riscv_simd_math_abi_v1(0xffff)));
#pragma omp declare simd notinbranch
extern float powf (float, float) __attribute__((riscv_simd_math_abi_v1(0xffff)));

void
logfloop (float *restrict y, const float *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = logf (x[i]);
}

void
powfloop (float *restrict y, const float *restrict x, const float *restrict z,
	  unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = powf (x[i], z[i]);
}

/* { dg-final { scan-assembler "call[ \t]+_ZGV8Nxv_logf" } } */
/* { dg-final { scan-assembler "call[ \t]+_ZGV8Nxvv_powf" } } */
