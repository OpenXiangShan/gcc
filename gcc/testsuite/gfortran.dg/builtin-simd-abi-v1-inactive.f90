! { dg-do compile { target riscv64*-*-linux* } }
! { dg-additional-options "-O2 -nostdinc -march=rv64gcv -mabi=lp64d" }

! When IF('fastmath') is false the clause is ignored for code generation,
! but its grammar, target availability and range must still be valid.

!GCC$ builtin (log) attributes simd (notinbranch) riscv_simd_math_abi_v1(51) if('fastmath')

program p
  real(8) :: x(32), y(32)
  read (*, *) x
  y = log (x)
  if (y(1) < -1.0d0) stop 1
end
