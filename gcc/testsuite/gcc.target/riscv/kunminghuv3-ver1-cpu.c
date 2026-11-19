/* { dg-do compile { target rv64 } } */
/* { dg-options "-mcpu=xiangshan-kunminghuv3-ver1 -mabi=lp64d" } */

#ifndef __riscv_v
#error "Kunminghu V3 must default to the RVA23 vector extension"
#endif
#ifndef __riscv_zvbb
#error "Kunminghu V3 must default to the RVA23 vector bitmanip extension"
#endif

int x;
