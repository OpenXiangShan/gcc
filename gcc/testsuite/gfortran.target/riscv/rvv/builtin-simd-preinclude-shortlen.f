! { dg-do compile }
! { dg-additional-files "builtin-simd-preinclude-fixed.h" }
! { dg-options "-nostdinc -march=rv64gcv -mabi=lp64d -ffixed-line-length-9 -ffast-math -fpre-include=builtin-simd-preinclude-fixed.h" }
! A valid fixed line length below the 15-character prefix must not discard
! the compiler preinclude record before it is recognized.
      end
