! { dg-do compile { target { x86_64*-*-linux* || riscv64*-*-linux* } } }
! { dg-additional-options "-O2 -nostdinc -fdump-ipa-simdclone" }
! { dg-additional-options "-march=rv64gcv -mabi=lp64d" { target riscv64*-*-linux* } }
! { dg-additional-options "-fno-tree-vectorize" { target x86_64*-*-linux* } }

! On x86 the vectorizer can select the masked clone and hit a pre-existing
! veclower2 ICE; -fno-tree-vectorize keeps this test on clause handling.
! RISC-V needs the ISA/ABI flags for the masked double-log clone.

! An explicit "(inbranch)" clause must produce only the masked clone
! ('M' in the ABI name).  A stray NOTINBRANCH clause here would be a
! regression of the single-clause case.

!GCC$ builtin (log) attributes simd (inbranch)

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end

! { dg-final { scan-ipa-dump {Generated global clone _ZGV[^\n]*M[^\n]*_log} "simdclone" } }
! { dg-final { scan-ipa-dump-not {Generated global clone _ZGV[^\n]*N[^\n]*_log} "simdclone" } }
