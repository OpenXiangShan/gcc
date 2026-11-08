! { dg-do run { target { riscv*-*-linux* } } }
! { dg-output "Fortran runtime warning: Floating-point synchronous exceptions are not supported on RISC-V[.]" }
! Explicit enabling must diagnose the request, not pretend to enable traps.
program no_trap_enable
  use ieee_exceptions
  implicit none
  logical :: flag, halting
  call ieee_set_flag(ieee_invalid, .true.)
  call ieee_set_halting_mode(ieee_invalid, .true.)
  call ieee_get_halting_mode(ieee_invalid, halting)
  if (halting) stop 1
  call ieee_get_flag(ieee_invalid, flag)
  if (.not. flag) stop 2
  call ieee_set_flag(ieee_invalid, .false.)
end program
