! { dg-do run { target { riscv*-*-linux* } } }
! RISC-V supports exception flags and rounding, but never synchronous traps.
program no_trap
  use ieee_arithmetic
  implicit none
  type(ieee_flag_type) :: flags(5)
  type(ieee_round_type) :: old_round, round
  type(ieee_status_type) :: status
  logical :: flag, halting
  integer :: i

  flags = [ieee_invalid, ieee_divide_by_zero, ieee_overflow, &
           ieee_underflow, ieee_inexact]
  do i = 1, size(flags)
    if (ieee_support_halting(flags(i))) stop 1
    call ieee_get_halting_mode(flags(i), halting)
    if (halting) stop 2
    if (.not. ieee_support_flag(flags(i))) stop 3
    call ieee_set_flag(flags(i), .true.)
    ! Disabling unsupported traps must not clear accrued exception flags.
    call ieee_set_halting_mode(flags(i), .false.)
    call ieee_get_flag(flags(i), flag)
    if (.not. flag) stop 4
    call ieee_set_flag(flags(i), .false.)
    call ieee_get_flag(flags(i), flag)
    if (flag) stop 5
  end do

  call ieee_get_rounding_mode(old_round)
  if (.not. ieee_support_rounding(ieee_up)) stop 6
  if (.not. ieee_support_rounding(ieee_down)) stop 7
  call ieee_set_rounding_mode(ieee_up)
  call ieee_get_rounding_mode(round)
  if (round /= ieee_up) stop 8
  call ieee_set_flag(ieee_invalid, .true.)
  call ieee_get_status(status)
  call ieee_set_rounding_mode(ieee_down)
  call ieee_set_flag(ieee_invalid, .false.)
  call ieee_set_status(status)
  call ieee_get_rounding_mode(round)
  if (round /= ieee_up) stop 9
  call ieee_get_flag(ieee_invalid, flag)
  if (.not. flag) stop 10
  call ieee_get_halting_mode(ieee_invalid, halting)
  if (halting) stop 11
  call ieee_set_flag(ieee_all, .false.)
  call ieee_set_rounding_mode(old_round)
end program
