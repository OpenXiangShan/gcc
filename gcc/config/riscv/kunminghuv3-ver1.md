;; XiangShan Kunminghu V3 (new-int), first revision.
;; Copyright (C) 2026 Free Software Foundation, Inc.
;;
;; This file is part of GCC.
;;
;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.
;;
;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; Vector topology, split counts and fixed EX result slots are from XiangShan
;; f2ca2052ee1357c36a9497bfc2a24fe9a85d0252 (single-core DefaultConfig) and
;; yunsuan e804cd323994c631b9f3f9986ed937e7cad612b6.  The fallback is the
;; tt-ascalon-x LLVM model at 6de76ff55b73148217b2a6abecde386d88cdd430,
;; not the tt-ascalon-d8 GCC model.  In particular, a result slot is not a
;; scheduler RAW latency.  Shift all vector result slots by the same constant
;; (omit issue-to-EX) for collision checks only.  Do not reserve a second
;; unit for the same fixed EX-result/writeback conflict.

(define_automaton "khv3_fx,khv3_fp,khv3_ls,khv3_vec,khv3_vstd,khv3_xvec")
(define_automaton "khv3_intdiv,khv3_fltdiv,khv3_intred,khv3_fltred,khv3_fltcvt")

;; Scalar pipeline fallback: FXA/FXB are single, FXC/FXD are dual, as is FP.
(define_cpu_unit "khv3_fxa,khv3_fxb,khv3_fxc0,khv3_fxc1,khv3_fxd0,khv3_fxd1"
  "khv3_fx")
(define_cpu_unit "khv3_fpa,khv3_fpb" "khv3_fp")
(define_cpu_unit "khv3_ls0,khv3_ls1,khv3_ls2,khv3_vld0,khv3_vld1" "khv3_ls")
(define_cpu_unit "khv3_vstd0,khv3_vstd1" "khv3_vstd")
(define_cpu_unit "khv3_i0,khv3_i1,khv3_i2,khv3_i3,khv3_o0,khv3_o1,khv3_o2,khv3_o3"
  "khv3_vec")
(define_cpu_unit "khv3_idiv" "khv3_intdiv")
(define_cpu_unit "khv3_fdiv" "khv3_fltdiv")
(define_cpu_unit "khv3_ired0,khv3_ired1" "khv3_intred")
(define_cpu_unit "khv3_fred" "khv3_fltred")
(define_cpu_unit "khv3_vfcvt,khv3_vimac" "khv3_fltcvt")
(define_cpu_unit "khv3_xv0,khv3_xv1" "khv3_xvec")

(define_reservation "khv3_branch" "khv3_fxc0|khv3_fxc1")
(define_reservation "khv3_fx_any"
  "khv3_fxa|khv3_fxb|khv3_branch|khv3_fxd0|khv3_fxd1")
(define_reservation "khv3_fp_any" "khv3_fpa|khv3_fpb")
(define_reservation "khv3_lsu" "khv3_ls0|khv3_ls1|khv3_ls2")
(define_reservation "khv3_vload" "khv3_lsu+(khv3_vld0|khv3_vld1)")
(define_reservation "khv3_vstore" "khv3_lsu+(khv3_vstd0|khv3_vstd1)")
(define_reservation "khv3_vi" "khv3_i0|khv3_i1|khv3_i2|khv3_i3")
(define_reservation "khv3_xv" "khv3_xv0|khv3_xv1")

;; i0--i3 are VFEX0/VEX1/VEX2/VEX3 issue slots; o0--o3 are their
;; independent shared result slots.  The suffix after "s" is the EX slot.
(define_reservation "khv3_u0s0" "khv3_i0+khv3_o0")
(define_reservation "khv3_u1s0" "khv3_i1+khv3_o1")
(define_reservation "khv3_u2s0" "khv3_i2+khv3_o2")
(define_reservation "khv3_u3s0" "khv3_i3+khv3_o3")
(define_reservation "khv3_u0s1" "khv3_i0,khv3_o0")
(define_reservation "khv3_u1s1" "khv3_i1,khv3_o1")
(define_reservation "khv3_u2s1" "khv3_i2,khv3_o2")
(define_reservation "khv3_u3s1" "khv3_i3,khv3_o3")
(define_reservation "khv3_u0s2" "khv3_i0,nothing,khv3_o0")
(define_reservation "khv3_u1s2" "khv3_i1,nothing,khv3_o1")
(define_reservation "khv3_u2s2" "khv3_i2,nothing,khv3_o2")
(define_reservation "khv3_u3s2" "khv3_i3,nothing,khv3_o3")
(define_reservation "khv3_u0s3" "khv3_i0,nothing*2,khv3_o0")
(define_reservation "khv3_u1s3" "khv3_i1,nothing*2,khv3_o1")
(define_reservation "khv3_u2s3" "khv3_i2,nothing*2,khv3_o2")
(define_reservation "khv3_u3s3" "khv3_i3,nothing*2,khv3_o3")

;; Fixed elementwise operations split by register slice.  GCC schedules an
;; entire RTL insn, not independently ready slices.  Bundle simultaneous
;; slices on distinct legal lanes and repeat groups for spills across cycles.
;; This models entry/output pressure, not chaining or architectural completion.
;; Pack two-slice bundles on either lane pair instead of enumerating every
;; pair.  Keep single-slice alternatives and exact slots within each bundle.
(define_reservation "khv3_all_i" "khv3_i0+khv3_i1+khv3_i2+khv3_i3")
(define_reservation "khv3_all_o" "khv3_o0+khv3_o1+khv3_o2+khv3_o3")
(define_reservation "khv3_mac_i" "khv3_i0+khv3_i1")
(define_reservation "khv3_mac_o" "khv3_o0+khv3_o1")

(define_reservation "khv3_s0_1"
  "khv3_u0s0|khv3_u1s0|khv3_u2s0|khv3_u3s0")
(define_reservation "khv3_s0_2"
  "khv3_u0s0+khv3_u1s0|khv3_u2s0+khv3_u3s0")
(define_reservation "khv3_s0_4"
  "khv3_u0s0+khv3_u1s0+khv3_u2s0+khv3_u3s0")
(define_reservation "khv3_s0_8"
  "khv3_s0_4*2")
(define_reservation "khv3_s1_1"
  "khv3_u0s1|khv3_u1s1|khv3_u2s1|khv3_u3s1")
(define_reservation "khv3_s1_2"
  "khv3_u0s1+khv3_u1s1|khv3_u2s1+khv3_u3s1")
(define_reservation "khv3_s1_4"
  "khv3_u0s1+khv3_u1s1+khv3_u2s1+khv3_u3s1")
(define_reservation "khv3_s1_8"
  "khv3_all_i*2+(nothing,khv3_all_o*2)")
(define_reservation "khv3_s2_1"
  "khv3_u0s2|khv3_u1s2|khv3_u2s2|khv3_u3s2")
(define_reservation "khv3_s2_2"
  "khv3_u0s2+khv3_u1s2|khv3_u2s2+khv3_u3s2")
(define_reservation "khv3_s2_4"
  "khv3_u0s2+khv3_u1s2+khv3_u2s2+khv3_u3s2")
(define_reservation "khv3_s2_8"
  "khv3_all_i*2+(nothing*2,khv3_all_o*2)")
(define_reservation "khv3_s3_1"
  "khv3_u0s3|khv3_u1s3|khv3_u2s3|khv3_u3s3")
(define_reservation "khv3_s3_2"
  "khv3_u0s3+khv3_u1s3|khv3_u2s3+khv3_u3s3")
(define_reservation "khv3_s3_4"
  "khv3_u0s3+khv3_u1s3+khv3_u2s3+khv3_u3s3")
(define_reservation "khv3_s3_8"
  "khv3_all_i*2+(nothing*3,khv3_all_o*2)")

(define_reservation "khv3_mac_1" "khv3_u0s2|khv3_u1s2")
(define_reservation "khv3_mac_2" "khv3_u0s2+khv3_u1s2")
(define_reservation "khv3_mac_4" "khv3_mac_i*2+(nothing*2,khv3_mac_o*2)")
(define_reservation "khv3_mac_8" "khv3_mac_4+khv3_vimac*4")

;; Split mode is based on the existing vtype mode (memory uses data
;; EEW/EMUL).  Unknown/fractional LMUL defaults to one register slice.
(define_attr "khv3_n" ""
  (cond [(match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_2") (const_int 2)
         (match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_4") (const_int 4)
         (match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_8") (const_int 8)]
        (const_int 1)))

;; Widening uses the destination EMUL for resource demand but the narrow
;; LMUL for the X latency fallback.  The GCC attribute uses narrow LMUL.
(define_attr "khv3_wide_n" ""
  (cond [(match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_1") (const_int 2)
         (match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_2") (const_int 4)
         (ior (match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_4")
              (match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_8")) (const_int 8)]
        (const_int 1)))

;; AscalonGetCyclesNarrowing: mf8/mf4=1, mf2=2, m1/m2/m4=4/8/16.
(define_attr "khv3_narrow_cost" ""
  (cond [(match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_1") (const_int 4)
         (match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_2") (const_int 8)
         (ior (match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_4")
              (match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_8")) (const_int 16)
         (match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_F2") (const_int 2)]
        (const_int 1)))

(define_attr "khv3_div_base" ""
  (cond [(match_test "get_attr_sew (insn) == 8") (const_int 7)
         (match_test "get_attr_sew (insn) == 16") (const_int 6)
         (match_test "get_attr_sew (insn) == 32") (const_int 5)]
        (const_int 8)))

;; X keys strided and indexed segments on the data EEW and per-field LMUL.
;; The GCC patterns use the data tuple mode, not the index mode.
(define_attr "khv3_seg_lmul" ""
  (cond [(match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_F8") (const_int 1)
         (match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_F4") (const_int 2)
         (match_test "get_attr_vlmul (insn) == riscv_vector::LMUL_F2") (const_int 4)]
        (symbol_ref "8 * get_attr_khv3_n (insn)")))

(define_attr "khv3_seg_eew" ""
  (cond [(match_test "get_attr_sew (insn) == 8") (const_int 4)
         (match_test "get_attr_sew (insn) == 16") (const_int 2)]
        (const_int 1)))

(define_attr "khv3_gather_scalar" "no,yes" (const_string "no"))

;; Use a single partition of TYPE, with per-pattern attributes for distinctions
;; not expressed in TYPE.  "other" covers scalar/unmapped forms (including
;; opaque multi-insns); "vother" is a one-cycle X-style placeholder for
;; target extensions and pseudos with no matching X row.
(define_attr "khv3_class"
  "none,other,branch,sload,sstore,fxa,fxb,mul,div,fp1,fp3,fpdiv,
   vload,vstore,load1,store1,mem_strided,seg_strided,seg_store,
   alu0,alu1,alu2,alu3,mac,vwide0,vwide1,vwide2,vwide3,wmac,
   nshift,nclip,cvt1,cvt2,wcvt1,wcvt2,ncvt1,ncvt2,mask,msfs,mpop,mfirst,iota,
   xmove_in,xmove_out,ired,fredu,fredo,idiv,fdiv,gather,gather_scalar,
   slide,vset,vother"
  (cond [
    (eq_attr "type" "ghost") (const_string "none")
    (eq_attr "type" "branch,jump,jalr,ret,call") (const_string "branch")
    (eq_attr "type" "load,fpload,atomic") (const_string "sload")
    (eq_attr "type" "store,fpstore") (const_string "sstore")
    (eq_attr "type" "fcvt_f2i,mfc,rdvlenb,rdvl,wrvxrm,wrfrm,rdfrm")
      (const_string "fxa")
    (eq_attr "type" "fcvt_i2f,mtc") (const_string "fxb")
    (eq_attr "type" "imul") (const_string "mul")
    (eq_attr "type" "idiv") (const_string "div")
    (eq_attr "type" "fcvt,fcmp") (const_string "fp1")
    (eq_attr "type" "fmove,fadd,fmul,fmadd") (const_string "fp3")
    (eq_attr "type" "fdiv,fsqrt") (const_string "fpdiv")

    (eq_attr "type" "vlde,vldff,vlsegde,vlsegdff") (const_string "vload")
    (eq_attr "type" "vste") (const_string "vstore")
    (eq_attr "type" "vldm,vldr") (const_string "load1")
    (eq_attr "type" "vstm,vstr") (const_string "store1")
    (eq_attr "type" "vlds,vldux,vldox,vsts,vstux,vstox")
      (const_string "mem_strided")
    (eq_attr "type" "vlsegds,vlsegdux,vlsegdox,vssegts,vssegtux,vssegtox")
      (const_string "seg_strided")
    (eq_attr "type" "vssegte") (const_string "seg_store")

    (eq_attr "type" "vialu,vicalu,vshift,vicmp,vext,vmidx,vaalu,vsshift,vandn,\
                     vimov,vmov,vimerge,vfmerge,vfmov") (const_string "alu0")
    (eq_attr "type" "vsalu,viminmax,vbrev,vbrev8,vrev8,vcpop,vclz,vctz,vrol,vror,\
                     vfalu,vfcmp,vfminmax,vfsgnj,vfclass") (const_string "alu1")
    (eq_attr "type" "vfmul") (const_string "alu2")
    (eq_attr "type" "vfmuladd") (const_string "alu3")
    (eq_attr "type" "vimul,vimuladd,vsmul") (const_string "mac")
    (eq_attr "type" "viwalu") (const_string "vwide0")
    (eq_attr "type" "vfwalu,vwsll") (const_string "vwide1")
    (eq_attr "type" "vfwmul") (const_string "vwide2")
    (eq_attr "type" "vfwmuladd,vfwmaccbf16") (const_string "vwide3")
    (eq_attr "type" "viwmul,viwmuladd") (const_string "wmac")
    (eq_attr "type" "vnshift") (const_string "nshift")
    (eq_attr "type" "vnclip") (const_string "nclip")
    (eq_attr "type" "vfcvtftoi,vfcvtitof")
      (if_then_else (match_test "get_attr_sew (insn) == 64") (const_string "cvt2")
                                         (const_string "cvt1"))
    (eq_attr "type" "vfrecp")
      (if_then_else (match_test "get_attr_sew (insn) == 64") (const_string "cvt2")
                                         (const_string "cvt1"))
    (eq_attr "type" "vfwcvtftoi,vfwcvtitof,vfwcvtftof,vfwcvtbf16")
      (if_then_else (match_test "get_attr_sew (insn) == 32") (const_string "wcvt2")
                                         (const_string "wcvt1"))
    (eq_attr "type" "vfncvtftoi,vfncvtitof,vfncvtftof,vfncvtbf16")
      (if_then_else (match_test "get_attr_sew (insn) == 32") (const_string "ncvt2")
                                         (const_string "ncvt1"))
    (eq_attr "type" "vmalu") (const_string "mask")
    (eq_attr "type" "vmsfs") (const_string "msfs")
    (eq_attr "type" "vmpop") (const_string "mpop")
    (eq_attr "type" "vmffs") (const_string "mfirst")
    (eq_attr "type" "vmiota") (const_string "iota")
    (eq_attr "type" "vimovxv,vfmovfv") (const_string "xmove_in")
    (eq_attr "type" "vimovvx,vfmovvf") (const_string "xmove_out")
    (eq_attr "type" "vired,viwred") (const_string "ired")
    (eq_attr "type" "vfredu,vfwredu") (const_string "fredu")
    (eq_attr "type" "vfredo,vfwredo") (const_string "fredo")
    (eq_attr "type" "vidiv") (const_string "idiv")
    (eq_attr "type" "vfdiv,vfsqrt") (const_string "fdiv")
    (and (eq_attr "type" "vgather") (eq_attr "khv3_gather_scalar" "yes"))
      (const_string "gather_scalar")
    (eq_attr "type" "vgather,vcompress") (const_string "gather")
    (eq_attr "type" "vslideup,vslidedown,vislide1up,vislide1down,vfslide1up,vfslide1down")
      (const_string "slide")
    (eq_attr "type" "vsetvl,vsetvl_pre") (const_string "vset")
    (eq_attr "type" "vector,vclmul,vclmulh,vghsh,vgmul,vaesef,vaesem,vaesdf,\
                     vaesdm,vaeskf1,vaeskf2,vaesz,vsha2ms,vsha2ch,vsha2cl,vsm4k,\
                     vsm4r,vsm3me,vsm3c,sf_vqmacc,sf_vfnrclip,sf_vc,sf_vc_se")
      (const_string "vother")]
    (const_string "other")))

(define_int_iterator KHV_N [1 2 4 8])
(define_int_iterator KHV_MEM_COST [2 4 8 16])
(define_int_iterator KHV_SEG_COST [1 2 4 8 16 32 64 128 256])
(define_int_iterator KHV_DIV_COST [5 6 7 8 10 12 14 16 20 24 28 32 40 48 56 64])
(define_int_iterator KHV_NARROW_COST [1 2 4 8 16])
(define_int_iterator KHV_GATHER_COST [5 7 15 51])
(define_int_iterator KHV_ORDERED_COST [18 36 72 144])
(define_int_attr khv3_repeat [(1 "") (2 "*2") (4 "*4") (8 "*8")])
(define_int_attr khv3_front_repeat [(1 "") (2 "*2") (4 "*2") (8 "*2")])
(define_int_attr khv3_half [(2 "1") (4 "2") (8 "4") (16 "8")])
(define_int_attr khv3_gather_n [(5 "1") (7 "2") (15 "4") (51 "8")])
(define_int_attr khv3_gather_hold [(5 "3") (7 "5") (15 "13") (51 "49")])
(define_int_attr khv3_ordered_n [(18 "1") (36 "2") (72 "4") (144 "8")])
(define_int_attr khv3_ordered_hold [(18 "18") (36 "36") (72 "64") (144 "64")])
(define_int_attr khv3_shrink_repeat [(1 "") (2 "") (4 "") (8 "*2") (16 "*4")])
(define_int_attr khv3_shrink_front [(1 "") (2 "") (4 "") (8 "*2") (16 "*2")])

;; Scalar fallback.
(define_insn_reservation "khv3_int" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "other"))
  "khv3_fx_any")

(define_insn_reservation "khv3_control" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "branch"))
  "khv3_branch")

(define_insn_reservation "khv3_load" 4
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "sload"))
  "khv3_lsu")

(define_insn_reservation "khv3_store" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "sstore"))
  "khv3_lsu")

(define_insn_reservation "khv3_fx_a" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "fxa"))
  "khv3_fxa")

(define_insn_reservation "khv3_fx_b" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "fxb"))
  "khv3_fxb")

(define_insn_reservation "khv3_mul" 3
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "mul"))
  "khv3_fxa")

(define_insn_reservation "khv3_div" 7
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "div"))
  "khv3_fxa*7")

(define_insn_reservation "khv3_fp_misc" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "fp1"))
  "khv3_fp_any")

(define_insn_reservation "khv3_fp" 3
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "fp3"))
  "khv3_fp_any")

(define_insn_reservation "khv3_fp_div32" 7
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "fpdiv")
            (eq_attr "mode" "!DF")))
  "khv3_fpa*7|khv3_fpb*7")

(define_insn_reservation "khv3_fp_div64" 12
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "fpdiv")
            (eq_attr "mode" "DF")))
  "khv3_fpa*12|khv3_fpb*12")

;; Vector memory intake: model the two vector load or store data ports
;; alongside the X LSU token.  Do not count split uops as L1 requests.
;; Latencies and the LSU lease duration come from X, not cache measurements.
(define_insn_reservation "khv3_vload_<KHV_N>" KHV_N
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "vload")
            (eq_attr "khv3_n" "<KHV_N>")))
  "khv3_vload")

(define_insn_reservation "khv3_vload_whole" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "load1"))
  "khv3_vload")

(define_insn_reservation "khv3_vstore" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "vstore,store1"))
  "khv3_vstore")

(define_insn_reservation "khv3_vmem_strided_<KHV_MEM_COST>" KHV_MEM_COST
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "mem_strided")
            (eq_attr "khv3_n" "<khv3_half>")))
  "khv3_lsu")

(define_insn_reservation "khv3_vsegment_strided_<KHV_SEG_COST>" KHV_SEG_COST
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "seg_strided")
            (eq (symbol_ref "get_attr_khv3_seg_lmul (insn)
                             * get_attr_khv3_seg_eew (insn)")
                 (const_int KHV_SEG_COST))))
  "khv3_lsu")

;; Segment-store X acquire at cycle 1; use a fixed LSU lane for its lease.
;; Segment stores have no data result.  Their modeled cost is not a
;; store-buffer or global-visibility latency.
(define_insn_reservation "khv3_vsegment_store_<KHV_N>" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "seg_store")
            (eq (symbol_ref "get_attr_khv3_n (insn)") (const_int KHV_N))))
  "nothing,(khv3_ls0<khv3_repeat>|khv3_ls1<khv3_repeat>|khv3_ls2<khv3_repeat>)")

;; Fixed vector arithmetic: X RAW latency and RTL issue/result slots.
(define_insn_reservation "khv3_valu_s0_<KHV_N>" KHV_N
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "alu0")
            (eq_attr "khv3_n" "<KHV_N>")))
  "khv3_s0_<KHV_N>")

(define_insn_reservation "khv3_valu_s1_<KHV_N>" KHV_N
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "alu1")
            (eq_attr "khv3_n" "<KHV_N>")))
  "khv3_s1_<KHV_N>")

(define_insn_reservation "khv3_valu_s2_<KHV_N>" KHV_N
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "alu2")
            (eq_attr "khv3_n" "<KHV_N>")))
  "khv3_s2_<KHV_N>")

(define_insn_reservation "khv3_valu_s3_<KHV_N>" KHV_N
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "alu3")
            (eq_attr "khv3_n" "<KHV_N>")))
  "khv3_s3_<KHV_N>")

(define_insn_reservation "khv3_vmac_<KHV_N>" KHV_N
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "mac")
            (eq_attr "khv3_n" "<KHV_N>")))
  "khv3_mac_<KHV_N>")

;; Express the wider bundle with a second iterator.  Its numeric use is
;; the (conservative) number of slices and not the default latency.
(define_int_iterator KHV_WIDE [1 2])

(define_insn_reservation "khv3_vwide0_short_<KHV_WIDE>" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "vwide0")
            (and (eq_attr "khv3_n" "1")
                 (eq (symbol_ref "get_attr_khv3_wide_n (insn)") (const_int KHV_WIDE)))))
  "khv3_s0_<KHV_WIDE>")

(define_insn_reservation "khv3_vwide0_4" 2
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "vwide0")
            (eq_attr "khv3_wide_n" "4")))
  "khv3_s0_4")

(define_insn_reservation "khv3_vwide0_8" 4
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "vwide0")
            (eq_attr "khv3_wide_n" "8")))
  "khv3_s0_8")

(define_insn_reservation "khv3_vwide1_short_<KHV_WIDE>" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "vwide1")
            (and (eq_attr "khv3_n" "1")
                 (eq (symbol_ref "get_attr_khv3_wide_n (insn)") (const_int KHV_WIDE)))))
  "khv3_s1_<KHV_WIDE>")

(define_insn_reservation "khv3_vwide1_4" 2
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "vwide1")
            (eq_attr "khv3_wide_n" "4")))
  "khv3_s1_4")

(define_insn_reservation "khv3_vwide1_8" 4
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "vwide1")
            (eq_attr "khv3_wide_n" "8")))
  "khv3_s1_8")

(define_insn_reservation "khv3_vwide2_short_<KHV_WIDE>" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "vwide2")
            (and (eq_attr "khv3_n" "1")
                 (eq (symbol_ref "get_attr_khv3_wide_n (insn)") (const_int KHV_WIDE)))))
  "khv3_s2_<KHV_WIDE>")

(define_insn_reservation "khv3_vwide2_4" 2
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "vwide2")
            (eq_attr "khv3_wide_n" "4")))
  "khv3_s2_4")

(define_insn_reservation "khv3_vwide2_8" 4
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "vwide2")
            (eq_attr "khv3_wide_n" "8")))
  "khv3_s2_8")

(define_insn_reservation "khv3_vwide3_short_<KHV_WIDE>" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "vwide3")
            (and (eq_attr "khv3_n" "1")
                 (eq (symbol_ref "get_attr_khv3_wide_n (insn)") (const_int KHV_WIDE)))))
  "khv3_s3_<KHV_WIDE>")

(define_insn_reservation "khv3_vwide3_4" 2
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "vwide3")
            (eq_attr "khv3_wide_n" "4")))
  "khv3_s3_4")

(define_insn_reservation "khv3_vwide3_8" 4
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "vwide3")
            (eq_attr "khv3_wide_n" "8")))
  "khv3_s3_8")

(define_insn_reservation "khv3_wmac_short_<KHV_WIDE>" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "wmac")
            (and (eq_attr "khv3_n" "1")
                 (eq (symbol_ref "get_attr_khv3_wide_n (insn)") (const_int KHV_WIDE)))))
  "khv3_mac_<KHV_WIDE>")

(define_insn_reservation "khv3_wmac_4" 2
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "wmac")
            (eq_attr "khv3_wide_n" "4")))
  "khv3_mac_4")

(define_insn_reservation "khv3_wmac_8" 4
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "wmac")
            (eq_attr "khv3_wide_n" "8")))
  "khv3_mac_8")

(define_insn_reservation "khv3_wcvt1_short_<KHV_WIDE>" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "wcvt1")
            (and (eq_attr "khv3_n" "1")
                 (eq (symbol_ref "get_attr_khv3_wide_n (insn)") (const_int KHV_WIDE)))))
  "khv3_vfcvt<khv3_repeat>+khv3_i0<khv3_front_repeat>+
   (nothing,khv3_o0<khv3_front_repeat>)")

(define_insn_reservation "khv3_wcvt1_4" 2
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "wcvt1")
            (eq_attr "khv3_wide_n" "4")))
  "khv3_vfcvt*4+khv3_i0*2+
   (nothing,khv3_o0*2)")

(define_insn_reservation "khv3_wcvt1_8" 4
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "wcvt1")
            (eq_attr "khv3_wide_n" "8")))
  "khv3_vfcvt*8+khv3_i0*2+
   (nothing,khv3_o0*2)")

(define_insn_reservation "khv3_wcvt2_short_<KHV_WIDE>" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "wcvt2")
            (and (eq_attr "khv3_n" "1")
                 (eq (symbol_ref "get_attr_khv3_wide_n (insn)") (const_int KHV_WIDE)))))
  "khv3_vfcvt<khv3_repeat>+khv3_i0<khv3_front_repeat>+
   (nothing*2,khv3_o0<khv3_front_repeat>)")

(define_insn_reservation "khv3_wcvt2_4" 2
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "wcvt2")
            (eq_attr "khv3_wide_n" "4")))
  "khv3_vfcvt*4+khv3_i0*2+
   (nothing*2,khv3_o0*2)")

(define_insn_reservation "khv3_wcvt2_8" 4
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "wcvt2")
            (eq_attr "khv3_wide_n" "8")))
  "khv3_vfcvt*8+khv3_i0*2+
   (nothing*2,khv3_o0*2)")

;; Integer narrowing can use two dependent halves for an integral LMUL.
;; Approximate separate issue of the two halves by a consecutive bundle
;; on the same lanes.  The X latency is for the entire insn, not the spacing of the halves.
(define_insn_reservation "khv3_nshift_<KHV_NARROW_COST>" KHV_NARROW_COST
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "nshift")
            (eq_attr "khv3_narrow_cost" "<KHV_NARROW_COST>")))
  "khv3_n_nshift_<KHV_NARROW_COST>")

(define_reservation "khv3_n_nshift_1" "khv3_s0_1")
(define_reservation "khv3_n_nshift_2" "khv3_s0_1")
(define_reservation "khv3_n_nshift_4" "khv3_u0s0*2|khv3_u1s0*2|khv3_u2s0*2|khv3_u3s0*2")
(define_reservation "khv3_n_nshift_8" "(khv3_u0s0+khv3_u1s0)*2|(khv3_u2s0+khv3_u3s0)*2")
(define_reservation "khv3_n_nshift_16" "khv3_s0_4*2")

(define_insn_reservation "khv3_nclip_<KHV_NARROW_COST>" KHV_NARROW_COST
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "nclip")
            (eq_attr "khv3_narrow_cost" "<KHV_NARROW_COST>")))
  "khv3_n_nclip_<KHV_NARROW_COST>")

(define_reservation "khv3_n_nclip_1" "khv3_s1_1")
(define_reservation "khv3_n_nclip_2" "khv3_s1_1")
(define_reservation "khv3_n_nclip_4" "(khv3_i0*2+(nothing,khv3_o0*2))|
   (khv3_i1*2+(nothing,khv3_o1*2))|
   (khv3_i2*2+(nothing,khv3_o2*2))|
   (khv3_i3*2+(nothing,khv3_o3*2))")
(define_reservation "khv3_n_nclip_8" "((khv3_i0+khv3_i1)*2+(nothing,(khv3_o0+khv3_o1)*2))|
   ((khv3_i2+khv3_i3)*2+(nothing,(khv3_o2+khv3_o3)*2))")
(define_reservation "khv3_n_nclip_16" "khv3_s1_8")

;; VFCvt and VIota use VFEX0.  Keep the first two slices on its issue and
;; result slots.  Track the full intake span in a separate lease; later
;; slices compete only with these operations, not with other VFEX0 users.
;; Narrow FCvt counts one slice per narrow register.
(define_insn_reservation "khv3_cvt_s1_<KHV_N>" KHV_N
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "cvt1")
            (eq_attr "khv3_n" "<KHV_N>")))
  "khv3_vfcvt<khv3_repeat>+khv3_i0<khv3_front_repeat>+
   (nothing,khv3_o0<khv3_front_repeat>)")

(define_insn_reservation "khv3_ncvt_s1_<KHV_NARROW_COST>" KHV_NARROW_COST
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "ncvt1")
            (eq_attr "khv3_narrow_cost" "<KHV_NARROW_COST>")))
  "khv3_vfcvt<khv3_shrink_repeat>+khv3_i0<khv3_shrink_front>+
   (nothing,khv3_o0<khv3_shrink_front>)")

(define_insn_reservation "khv3_cvt_s2_<KHV_N>" KHV_N
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "cvt2")
            (eq_attr "khv3_n" "<KHV_N>")))
  "khv3_vfcvt<khv3_repeat>+khv3_i0<khv3_front_repeat>+
   (nothing*2,khv3_o0<khv3_front_repeat>)")

(define_insn_reservation "khv3_ncvt_s2_<KHV_NARROW_COST>" KHV_NARROW_COST
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "ncvt2")
            (eq_attr "khv3_narrow_cost" "<KHV_NARROW_COST>")))
  "khv3_vfcvt<khv3_shrink_repeat>+khv3_i0<khv3_shrink_front>+
   (nothing*2,khv3_o0<khv3_shrink_front>)")

(define_insn_reservation "khv3_mask_logic" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "mask"))
  "khv3_s0_1")

(define_insn_reservation "khv3_mask_prefix" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "msfs"))
  "khv3_u0s1")

(define_insn_reservation "khv3_mask_pop" 2
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "mpop"))
  "khv3_u0s2")

(define_insn_reservation "khv3_mask_first" 2
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "mfirst"))
  "khv3_u0s1")

(define_insn_reservation "khv3_mask_iota_<KHV_N>" KHV_N
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "iota")
            (eq_attr "khv3_n" "<KHV_N>")))
  "khv3_vfcvt<khv3_repeat>+khv3_i0<khv3_front_repeat>+
   (nothing*2,khv3_o0<khv3_front_repeat>)")

(define_insn_reservation "khv3_scalar_insert" 2
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "xmove_in"))
  "khv3_s0_1")

(define_insn_reservation "khv3_scalar_extract" 2
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "xmove_out"))
  "khv3_u0s0")

;; Variable-time units: combine the known route with an X occupancy/RAW
;; proxy.  Do not reserve a fixed WB slot for divider grants or FP reduction.
(define_insn_reservation "khv3_vidiv_<KHV_DIV_COST>" KHV_DIV_COST
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "idiv")
            (eq (symbol_ref "get_attr_khv3_n (insn)
                             * get_attr_khv3_div_base (insn)")
                 (const_int KHV_DIV_COST))))
  "khv3_i1+khv3_fx_any+khv3_idiv*<KHV_DIV_COST>")

(define_insn_reservation "khv3_vfdiv_<KHV_DIV_COST>" KHV_DIV_COST
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "fdiv")
            (eq (symbol_ref "get_attr_khv3_n (insn)
                             * get_attr_khv3_div_base (insn)")
                 (const_int KHV_DIV_COST))))
  "khv3_i2+khv3_fp_any+khv3_fdiv*<KHV_DIV_COST>")

(define_insn_reservation "khv3_vired_<KHV_N>" KHV_N
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "ired")
            (eq_attr "khv3_n" "<KHV_N>")))
  "khv3_fx_any+
   ((khv3_i0+khv3_ired0<khv3_repeat>)|(khv3_i1+khv3_ired1<khv3_repeat>))")

(define_insn_reservation "khv3_vfred_unordered_<KHV_N>" KHV_N
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "fredu")
            (eq_attr "khv3_n" "<KHV_N>")))
  "khv3_i0+khv3_fx_any+khv3_fred<khv3_repeat>")

;; Cap the last two ordered-reduction leases at the 64-cycle divider limit.
;; The generator's global state width follows the longest reservation, not
;; the dependency latency.  Keep X's full RAW fallback at every LMUL.
(define_insn_reservation "khv3_vfred_ordered_<KHV_ORDERED_COST>" KHV_ORDERED_COST
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "fredo")
            (eq_attr "khv3_n" "<khv3_ordered_n>")))
  "khv3_i0+khv3_fx_any+khv3_fred*<khv3_ordered_hold>")

;; Permute fallback: X's two abstract leases are not two physical PermUnits.
;; Known four-way ingress still competes with fixed vector instructions.
;; Model VSETVLI/IVLI with the same one-cycle X token; GCC does not have an
;; ordinary register-VTYPE VSETVL RTL pattern here.  Do not predict when an
;; instruction using a changed vtype or a nonzero AVL will actually issue.
(define_insn_reservation "khv3_gather_vv_<KHV_GATHER_COST>" KHV_GATHER_COST
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "gather")
            (eq_attr "khv3_n" "<khv3_gather_n>")))
  "khv3_vi+khv3_fx_any+
   (khv3_xv0*<khv3_gather_hold>|khv3_xv1*<khv3_gather_hold>)")

;; Keep the X vector lease and project its multi-cycle FX lease to one
;; ingress cycle, avoiding a product of six independent long FX leases.
(define_insn_reservation "khv3_gather_x_<KHV_MEM_COST>" KHV_MEM_COST
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "gather_scalar")
            (eq_attr "khv3_n" "<khv3_half>")))
  "khv3_vi+khv3_fx_any+
   (khv3_xv0*<KHV_MEM_COST>|khv3_xv1*<KHV_MEM_COST>)")

(define_insn_reservation "khv3_slide_<KHV_N>" 4
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (and (eq_attr "khv3_class" "slide")
            (eq (symbol_ref "get_attr_khv3_n (insn)") (const_int KHV_N))))
  "khv3_vi+khv3_fx_any,(khv3_xv0<khv3_repeat>|khv3_xv1<khv3_repeat>)")

(define_insn_reservation "khv3_vset" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "vset"))
  "khv3_xv")

(define_insn_reservation "khv3_vother" 1
  (and (eq_attr "tune" "kunminghuv3_ver1")
       (eq_attr "khv3_class" "vother"))
  "khv3_vi+khv3_xv")
