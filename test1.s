/* for passing parameter load registers r0, r1 ..
 * accordingly for 1st, 2nd paramters and so on 
 
 * For returning a value, load r0 register
 */
 
    .code   16              @@ 16 bit thumb mode
    .text                   @@ - Code section (text -> ROM)

@@ <function block>
    .align  2               @@ - 2^n alignment (n=2)
    .syntax unified
    .global myadd          @@ - Symbol name for function
    .code   16              @@ - 16bit THUMB code (BOTH are required!)
    .thumb_func             @@ /
    .type   myadd, %function   @@ - symbol type (not req)

@@ Declaration : int myadd(int x)
@@ Uses r0 for param 0
@@   r0: x
myadd:
    push {lr}
    // mov r1, #3
    push {r0-r7}
    bl   CmdExample           @@ call BSP function
    pop  {r0-r7}
    add r0, r0, r1
    pop {pc}
