/* { dg-do compile { target rv64 } } */
/* { dg-options "-march=rv64gc -mcpu=xiangshan-kunminghuv3-ver1 -mabi=lp64d -mtune=generic-ooo" } */

#ifdef __riscv_v
#error "Explicit -march=rv64gc must override the CPU's default ISA"
#endif

int x;
