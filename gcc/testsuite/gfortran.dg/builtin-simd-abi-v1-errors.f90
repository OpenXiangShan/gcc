! { dg-do compile { target riscv64*-*-linux* } }
! { dg-additional-options "-O2 -nostdinc -march=rv64gcv -mabi=lp64d" }

! Malformed ABI masks must be diagnosed strictly.

!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(-1) ! { dg-error "Expected a decimal integer constant" }
!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(+1) ! { dg-error "Expected a decimal integer constant" }
!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(0x10) ! { dg-error "after 'riscv_simd_math_abi_v1' mask" }
!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(1_8) ! { dg-error "after 'riscv_simd_math_abi_v1' mask" }
!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(1.0) ! { dg-error "after 'riscv_simd_math_abi_v1' mask" }
!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(256) ! { dg-error "outside the range 0 to 255" }
!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(51 ! { dg-error "after 'riscv_simd_math_abi_v1' mask" }
!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1 ! { dg-error "after 'riscv_simd_math_abi_v1' at" }
!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(51) junk ! { dg-error "Unexpected text" }
!GCC$ builtin (log) attributes simd riscv_simd_math_abi_v1(51) riscv_simd_math_abi_v1(15) ! { dg-error "Unexpected text" }
!GCC$ builtin (log) attributes simd if('fastmath') riscv_simd_math_abi_v1(51) ! { dg-error "must appear before" }

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end
