! { dg-do compile }
! { dg-additional-files "builtin-simd-preinclude-fixed.h" }
! { dg-options "-nostdinc -march=rv64gcv -mabi=lp64d -ffree-line-length-8 -ffast-math -fpre-include=builtin-simd-preinclude-fixed.h" }
! The same below-prefix length rule applies in free form.
end
