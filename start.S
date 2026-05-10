.section ".init"

_start:
    mrs   x1, mpidr_el1
    and   x1, x1, #3
    cbnz  x1, cpu_x

cpu_0:
    ldr  x2, =_start + 0x400
    mov  sp, x2     
    b  main

cpu_x:
    wfe
    b cpu_x    