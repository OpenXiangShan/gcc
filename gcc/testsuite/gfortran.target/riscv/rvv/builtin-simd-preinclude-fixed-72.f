! { dg-do compile }
! { dg-additional-files "builtin-simd-preinclude-fixed.h" }
! { dg-options "-nostdinc -march=rv64gcv -mabi=lp64d -ffixed-line-length-72 -ffast-math -fpre-include=builtin-simd-preinclude-fixed.h" }
! The compiler-provided preinclude record above is 92 columns.  This test
! must keep it intact even though the fixed-form user limit is 72.
      end
