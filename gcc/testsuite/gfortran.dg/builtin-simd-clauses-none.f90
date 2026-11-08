! { dg-do compile { target { x86_64*-*-linux* || riscv64*-*-linux* } } }
! { dg-additional-options "-O2 -nostdinc -fdump-ipa-simdclone" }
! { dg-additional-options "-march=rv64gcv -mabi=lp64d" { target riscv64*-*-linux* } }
! { dg-additional-options "-fno-tree-vectorize" { target x86_64*-*-linux* } }

! On x86 the vectorizer can select the masked clone and hit a pre-existing
! veclower2 ICE; -fno-tree-vectorize keeps this test on clause handling.
! RISC-V needs the ISA/ABI flags for the masked double-log clone.

! A bare "attributes simd" directive attaches an "omp declare simd"
! attribute without any clause (SIMD_NONE).  The simd clone pass must
! then create both the inbranch (masked, 'M' in the clone name) and the
! notinbranch ('N') clone.  This guards the pre-existing NONE behavior.
!
! log real(8) is used because it has a masked clone ABI on RISC-V; x86
! provides the standard b/c/d/e variants.  The checks are on ABI names,
! not on target registers.

!GCC$ builtin (log) attributes simd

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end

! { dg-final { scan-ipa-dump {Generated global clone _ZGV[^\n]*M[^\n]*_log} "simdclone" } }
! { dg-final { scan-ipa-dump {Generated global clone _ZGV[^\n]*N[^\n]*_log} "simdclone" } }
