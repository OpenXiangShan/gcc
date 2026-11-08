! { dg-do compile { target riscv64*-*-linux* } }
! { dg-additional-options "-O2 -nostdinc -march=rv64gcv -mabi=lp64d -fdump-ipa-simdclone" }

! Without ABI metadata the legacy m2 clone remains unchanged.

!GCC$ builtin (log) attributes simd (notinbranch)

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end

! { dg-final { scan-ipa-dump "_ZGV2Nxv_log" "simdclone" } }
! { dg-final { scan-ipa-dump-not "_ZGV1Nxv_log" "simdclone" } }
! { dg-final { scan-ipa-dump-not {_ZGV[^\n]*M[^\n]*_log} "simdclone" } }
! { dg-final { scan-ipa-dump-times {Generated global clone _ZGV[^\n]*_log} 1 "simdclone" } }
