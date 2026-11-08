! { dg-do compile { target riscv64*-*-linux* } }
! { dg-additional-options "-O2 -nostdinc -march=rv64gcv -mabi=lp64d -fdump-ipa-simdclone" }

! A bare simd directive with an explicit zero mask must create no clone,
! without relying on a later legacy directive to preserve the zero.

!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(0)

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end

! { dg-final { scan-ipa-dump-not {_ZGV[^\n]*_log} "simdclone" } }
