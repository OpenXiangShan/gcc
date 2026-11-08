! { dg-do compile { target { x86_64*-*-linux* || riscv64*-*-linux* } } }
! { dg-additional-options "-O2 -nostdinc -fdump-ipa-simdclone" }
! { dg-additional-options "-march=rv64gcv -mabi=lp64d" { target riscv64*-*-linux* } }
! { dg-additional-options "-fno-tree-vectorize" { target x86_64*-*-linux* } }

! On x86 the vectorizer can select the masked clone and hit a pre-existing
! veclower2 ICE; -fno-tree-vectorize keeps this test on clause handling.
! RISC-V needs the ISA/ABI flags for the masked double-log clone.

! Two directives for the same built-in OR their clause bits together
! (decl.cc: gfc_match_gcc_builtin).  With both INBRANCH and NOTINBRANCH
! requested, both the masked ('M') and the unmasked ('N') clone must be
! generated.  trans-intrinsic.cc add_simd_flag_for_built_in used to cast
! the whole mask instead of the individual bit, emitting two
! NOTINBRANCH attributes and losing the inbranch clone.

!GCC$ builtin (log) attributes simd (inbranch)
!GCC$ builtin (log) attributes simd (notinbranch)

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end

! { dg-final { scan-ipa-dump {Generated global clone _ZGV[^\n]*M[^\n]*_log} "simdclone" } }
! { dg-final { scan-ipa-dump {Generated global clone _ZGV[^\n]*N[^\n]*_log} "simdclone" } }
