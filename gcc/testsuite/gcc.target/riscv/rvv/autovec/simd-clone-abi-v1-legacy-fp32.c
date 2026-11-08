/* { dg-do compile { target { rv64 } } } */
/* { dg-options "-O3 -ffast-math -fopenmp-simd -march=rv64gc_zve32f_zvl128b -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2" } */

/* Without ABI v1 metadata, preserve the old requirement that the legacy
   contract only applies to vector-FP64 configurations.  */
#pragma omp declare simd notinbranch
extern float logf (float);

void
apply (float *restrict y, const float *restrict x, unsigned long n)
{
  for (unsigned long i = 0; i < n; ++i)
    y[i] = logf (x[i]);
}

/* { dg-final { scan-assembler-not "_ZGV" } } */
/* { dg-final { scan-assembler "call[ \t]+logf" } } */
