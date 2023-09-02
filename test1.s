    .code   16              @@ 16 bit thumb mode
    .text                   @@ - Code section (text -> ROM)

@@ <function block>
    .align  2               @@ - 2^n alignment (n=2)
    .syntax unified
    .global myadd          @@ - Symbol name for function
    .code   16              @@ - 16bit THUMB code (BOTH are required!)
    .thumb_func             @@ /
    .type   myadd, %function   @@ - symbol type (not req)

myadd:
    mov r1, #1
    add r0, r1, #2

