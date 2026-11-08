! { dg-do run { target { riscv*-*-linux* } } }
! { dg-additional-options "-ffpe-trap=invalid,zero,overflow,underflow,inexact,denormal -ffpe-summary=none" }
! { dg-output "Fortran runtime warning: Floating-point synchronous exceptions are not supported on RISC-V[.]" }
program no_trap_option
  use ieee_exceptions
  implicit none
  logical :: halting
  if (ieee_support_halting(ieee_invalid)) stop 1
  call ieee_get_halting_mode(ieee_invalid, halting)
  if (halting) stop 2
end program
