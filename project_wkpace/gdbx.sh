define rst
  monitor reset
end

define ld
  target remote localhost:2331
  monitor reset
  load
end

define crash
  set $esf = $arg0
  set $callee = &($esf->extra_info)
  set $r0 = ((const struct __basic_sf *) $esf)->a1
  set $r1 = ((const struct __basic_sf *) $esf)->a2
  set $r2 = ((const struct __basic_sf *) $esf)->a3
  set $r3 = ((const struct __basic_sf *) $esf)->a4
  set $r4 = ((const _callee_saved_t *) $callee)->v1
  set $r5 = ((const _callee_saved_t *) $callee)->v2
  set $r6 = ((const _callee_saved_t *) $callee)->v3
  set $r7 = ((const _callee_saved_t *) $callee)->v4
  set $r8 = ((const _callee_saved_t *) $callee)->v5
  set $r9 = ((const _callee_saved_t *) $callee)->v6
  set $r10 = ((const _callee_saved_t *) $callee)->v7
  set $r11 = ((const _callee_saved_t *) $callee)->v8
  set $r12 = ((const struct __basic_sf *) $esf)->ip
  set $msp = ((const _callee_saved_t *) $callee)->psp
  set $lr = ((const struct __basic_sf *) $esf)->lr
  set $pc = ((const struct __basic_sf *) $esf)->pc
  set $cpsr = ((const struct __basic_sf *) $esf)->xpsr
  bt
end
