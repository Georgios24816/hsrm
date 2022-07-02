.LC0:
        .ascii  "HELLO\000"
main:
        push    {r4}
        movs    r4, #72
        ldr     r2, .L20
        ldr     r3, .L20+4
        sub     sp, sp, #12
        ldm     r2, {r0, r1}
        str     r0, [sp]
        strh    r1, [sp, #4]    @ movhi
        str     r4, [r3]
        ldrb    r2, [sp, #1]    @ zero_extendqisi2
        cbz     r2, .L2
        str     r2, [r3]
        ldrb    r2, [sp, #2]    @ zero_extendqisi2
        cbz     r2, .L2
        str     r2, [r3]
        ldrb    r2, [sp, #3]    @ zero_extendqisi2
        cbz     r2, .L2
        str     r2, [r3]
        ldrb    r2, [sp, #4]    @ zero_extendqisi2
        cbz     r2, .L2
        str     r2, [r3]
        ldrb    r2, [sp, #5]    @ zero_extendqisi2
        cbz     r2, .L2
        str     r2, [r3]
.L2:
        movs    r0, #0
        add     sp, sp, #12
        pop     {r4}
        bx      lr
.L20:
        .word   .LC0
        .word   1073790976
