! { dg-do compile }
! { dg-options "-nostdinc -march=rv64gcv -mabi=lp64d -ffixed-line-length-72 -ffast-math" }
! { dg-error "Expected" "" { target *-*-* } 5 }
! Ordinary user source keeps column-72 truncation; no global exemption.
!GCC$ builtin (log) attributes simd (notinbranch) riscv_simd_math_abi_v1(255) if('fastmath')
      end
