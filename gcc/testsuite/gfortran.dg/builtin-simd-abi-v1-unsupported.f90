! { dg-do compile { target x86_64*-*-linux* } }
! { dg-additional-options "-O2 -nostdinc -fno-tree-vectorize" }

! The RISC-V SIMD math ABI attribute is not available on other targets.
! The new clause must be rejected instead of being silently ignored.

!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(51) ! { dg-error "not supported for this target" }

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end
