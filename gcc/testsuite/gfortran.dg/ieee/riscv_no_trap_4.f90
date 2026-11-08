! { dg-do run { target { riscv*-*-linux* } } }
! Initialization expressions must agree with runtime capability queries,
! including renamed imports and module re-exports.
module no_trap_constants
  use ieee_arithmetic, only: ieee_invalid, ieee_overflow, ieee_nearest, &
       halting => ieee_support_halting, ieee_support_flag, ieee_support_rounding
  implicit none
  logical, parameter :: no_invalid = halting(ieee_invalid)
  logical, parameter :: no_overflow = halting(ieee_overflow)
  logical, parameter :: flags = ieee_support_flag(ieee_invalid)
  logical, parameter :: rounding = ieee_support_rounding(ieee_nearest)
  integer, parameter :: extent = merge(-1, 1, no_invalid .or. no_overflow)
end module

program constants
  use no_trap_constants
  implicit none
  integer :: array(extent)
  if (no_invalid .or. no_overflow) stop 1
  if (size(array) /= 1) stop 2
  if (.not. flags .or. .not. rounding) stop 3
end program
