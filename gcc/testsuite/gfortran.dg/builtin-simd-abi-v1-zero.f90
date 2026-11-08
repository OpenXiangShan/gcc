! { dg-do compile { target riscv64*-*-linux* } }
! { dg-additional-options "-O2 -nostdinc -march=rv64gcv -mabi=lp64d -fdump-ipa-simdclone" }

! An explicit mask of zero means "no ABI variants" and is distinct from
! the absence of ABI metadata.  A later legacy directive must not erase
! the explicit zero, and no SIMD clone may be created.

!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(0)
!GCC$ builtin (log) attributes simd (notinbranch)

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end

! { dg-final { scan-ipa-dump-not {_ZGV[^\n]*_log} "simdclone" } }
