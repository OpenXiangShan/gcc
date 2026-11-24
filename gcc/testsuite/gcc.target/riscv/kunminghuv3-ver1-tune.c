/* { dg-do compile { target rv64 } } */
/* { dg-options "-O2 -march=rv64gc -mabi=lp64d -mtune=xiangshan-kunminghuv3-ver1" } */

/* The kunminghu cost model (shared with -mtune=xiangshan-kunminghu) reports
   slow unaligned scalar accesses, so a packed 32-bit field is assembled from
   byte loads instead of a single lw.  This used to be a plain lw when the
   ver1 tune still borrowed generic_ooo_tune_info.  */
struct __attribute__ ((packed)) packed_s { char c; int i; };

int
get_i (const struct packed_s *s)
{
  return s->i;
}

/* { dg-final { scan-assembler-not {\mlw\M} } } */
/* { dg-final { scan-assembler {\mlbu\M} } } */
