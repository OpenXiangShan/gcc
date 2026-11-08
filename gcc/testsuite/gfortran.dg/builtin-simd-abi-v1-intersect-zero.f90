! { dg-do compile { target riscv64*-*-linux* } }
! { dg-additional-options "-O2 -nostdinc -march=rv64gcv -mabi=lp64d -fdump-ipa-simdclone" }

! Intersecting an explicit zero with a non-zero mask stays zero; the later
! mask must not overwrite the zero.

!GCC$ builtin (log) attributes simd (notinbranch) riscv_simd_math_abi_v1(0)
!GCC$ builtin (log) attributes simd (notinbranch) riscv_simd_math_abi_v1(51)

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end

! { dg-final { scan-ipa-dump-not {_ZGV[^\n]*_log} "simdclone" } }
