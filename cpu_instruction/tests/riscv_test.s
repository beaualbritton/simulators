jal x0, main
f1:
    addi    sp, sp, -32
    sw      ra, 28(sp)
    sw      s0, 24(sp)
    addi    s0, sp, 32
    sw      a0, 4(sp)
    sw      a1, 8(sp)
    sw      a2, 12(sp)
    sw      a3, 16(sp)
    sw      x0, 0(sp)
    lw      a0, 4(sp)
    lw      a1, 8(sp)
    lw      a2, 12(sp)
    bge     a1, x0, L1
    add     a2, a0, x0
    sw      a2, 0(sp)
    lw      a0, 0(sp)
    jal     x0, L2

L1:
    add     a2, a0, a1
    sw      a2, 0(sp)
    lw      a0, 0(sp)

L2:
    lw      s0, 24(sp)
    lw      ra, 28(sp)
    addi    sp, sp, 32
    jalr    x0, ra, 0

f2:
    addi    sp, sp, -32
    sw      ra, 28(sp)
    sw      s0, 24(sp)
    addi    s0, sp, 32
    sw      a0, -28(s0)
    sw      a1, -24(s0)
    addi    a0, x0, 0
    sw      a0, -16(s0)
    sw      x0, -20(s0)

loop:
    lw      a1, -28(s0)
    lw      a0, -16(s0)
    bge     a0, a1, after
    lw      a2, -24(s0)
    lw      a0, -16(s0)
    add     a0, a0, a0
    add     a0, a0, a0
    add     a0, a0, a2
    lw      a0, 0(a0)
    lw      a2, -20(s0)
    add     a2, a2, a0
    sw      a2, -20(s0)
    lw      a0, -16(s0)
    addi    a0, a0, 1
    sw      a0, -16(s0)
    jal     x0, loop

after:
    lw      a0, -20(s0)
    lw      s0, 24(sp)
    lw      ra, 28(sp)
    addi    sp, sp, 32
    jalr    x0, ra, 0

main:
    addi    sp, x0, 64
    addi    a0, x0, 12
    addi    a1, x0, 16
    jal     ra, f1
    addi    a2, a0, 0
    jal     ra, f2
