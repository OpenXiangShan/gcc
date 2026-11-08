! { dg-do compile { target riscv64*-*-linux* } }
! { dg-additional-options "-O2 -nostdinc -march=rv64gcv -mabi=lp64d -fdump-ipa-simdclone" }

! A bare simd directive normally requests both branches, but the explicit
! M-only mask 12 must create only the m1 masked clone.

!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(12)

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end

! { dg-final { scan-ipa-dump "_ZGV1Mxv_log" "simdclone" } }
! { dg-final { scan-ipa-dump-not {_ZGV[^\n]*N[^\n]*_log} "simdclone" } }
! { dg-final { scan-ipa-dump-not "_ZGV2Mxv_log" "simdclone" } }
! { dg-final { scan-ipa-dump-times {Generated global clone _ZGV[^\n]*_log} 1 "simdclone" } }
