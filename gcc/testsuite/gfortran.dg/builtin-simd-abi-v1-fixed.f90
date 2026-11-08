! { dg-do compile { target riscv64*-*-linux* } }
! { dg-additional-options "-O2 -nostdinc -march=rv64gcv -mabi=lp64d -mrvv-vector-bits=zvl -fdump-ipa-simdclone" }

! With fixed-size vectors the fixed-lane variants are selected.  Mask 255
! permits both branches and both LMULs at VLEN=128.

!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(255)

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end

! { dg-final { scan-ipa-dump "_ZGV1N2v_log" "simdclone" } }
! { dg-final { scan-ipa-dump "_ZGV1M2v_log" "simdclone" } }
! { dg-final { scan-ipa-dump "_ZGV2N4v_log" "simdclone" } }
! { dg-final { scan-ipa-dump "_ZGV2M4v_log" "simdclone" } }
! { dg-final { scan-ipa-dump-times {Generated global clone _ZGV[^\n]*_log} 4 "simdclone" } }
