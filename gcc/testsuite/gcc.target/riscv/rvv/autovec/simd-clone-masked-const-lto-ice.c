/* { dg-do link { target rv64 } } */
/* { dg-require-effective-target lto } */
/* { dg-require-effective-target riscv_v } */
/* { dg-options "-O2 -ffast-math -fopenmp-simd -fno-builtin-log -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -mrvv-max-lmul=m2 -flto -flto-partition=one -save-temps -lm" } */
/* { dg-additional-sources "simd-clone-masked-const-lto-ice-b.c" } */

/* The scalar declaration of log below has no const attribute, and
   -fno-builtin-log makes the calls use it.  The other TU declares log with
   const and an inbranch SIMD clone.  LTO merges the two declarations and
   drops const from the canonical one, so the masked SIMD clone call the
   vectorizer emits for the worker is non-const and clobbers memory.  The
   scalar statement it replaces is IFN_MASK_CALL, which is ECF_NOVOPS and has
   no virtual operands; the clone call cannot be given a chained virtual
   definition, so the delayed virtual SSA update must be requested.  This
   used to trip the assertion in vect_transform_loops.  */

#include <riscv_vector.h>

/* Resolve the clones locally so that the test does not depend on libmvec.  */
__attribute__((used, noinline, noclone)) vfloat64m2_t
_ZGV2M4v_log (vbool32_t mask, vfloat64m2_t x)
{
  (void) mask;
  return x;
}

__attribute__((used, noinline, noclone)) vfloat64m2_t
_ZGV2N4v_log (vfloat64m2_t x)
{
  return x;
}

extern void worker (double *, const double *);
extern double log (double);

static double x[64];
static double y[64];

int
main (int argc, char **argv)
{
  (void) argv;
  /* Keep the non-const scalar declaration of log alive: argc is unknown, so
     this call cannot be constant folded.  */
  y[0] = log ((double) argc + 2.0);
  for (int i = 0; i < 64; ++i)
    y[i] = log (x[i]);
  worker (y, x);
  return (int) y[0];
}

/* { dg-final { scan-lto-assembler "call[ \t]+_ZGV2M4v_log" } } */
