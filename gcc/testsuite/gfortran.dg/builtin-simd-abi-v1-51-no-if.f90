! { dg-do compile { target riscv64*-*-linux* } }
! { dg-additional-options "-O2 -nostdinc -march=rv64gcv -mabi=lp64d -fdump-ipa-simdclone" }

! The ABI clause is valid without IF.  Mask 51 selects the m1 and m2
! not-in-branch variants.

!GCC$ builtin (log) attributes simd (notinbranch) riscv_simd_math_abi_v1(51)

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end

! { dg-final { scan-ipa-dump "_ZGV1Nxv_log" "simdclone" } }
! { dg-final { scan-ipa-dump "_ZGV2Nxv_log" "simdclone" } }
! { dg-final { scan-ipa-dump-not {_ZGV[^\n]*M[^\n]*_log} "simdclone" } }
! { dg-final { scan-ipa-dump-times {Generated global clone _ZGV[^\n]*_log} 2 "simdclone" } }
