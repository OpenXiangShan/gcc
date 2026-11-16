! { dg-do compile { target riscv64*-*-linux* } }
! { dg-additional-options "-O2 -nostdinc -march=rv64gcv -mabi=lp64d -fdump-ipa-simdclone" }

! Mask 255 selects the LMUL1 and LMUL2 nibbles.  A bare simd directive
! requests both branches, and all bits in those two nibbles are set, so
! exactly four clones must be created.  The default -mrvv-max-lmul=m1 does
! not propose LMUL4 or LMUL8 clones, so the wider 16-bit mask does not add
! any clone here.

!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(255)

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end

! { dg-final { scan-ipa-dump "_ZGV1Nxv_log" "simdclone" } }
! { dg-final { scan-ipa-dump "_ZGV1Mxv_log" "simdclone" } }
! { dg-final { scan-ipa-dump "_ZGV2Nxv_log" "simdclone" } }
! { dg-final { scan-ipa-dump "_ZGV2Mxv_log" "simdclone" } }
! { dg-final { scan-ipa-dump-times {Generated global clone _ZGV[^\n]*_log} 4 "simdclone" } }
