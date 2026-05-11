.section ".init"
.globl _start

_start:
    // Only CPU0 reaches here initially with armstub
    // CPU1/2/3 are parked in armstub at 0x78
    
    mrs   x1, mpidr_el1
    and   x1, x1, #0x3
    cbnz  x1, secondary_entry    // Secondary cores jump here when woken
    
cpu_0:
    // Set up stack for CPU0
    ldr   x2, =_start
    add   x2, x2, #0x400
    mov   sp, x2
    
    // Wake up CPU1 by writing to its mailbox at 0xE0
    mov   x2, #0xE0
    ldr   x3, =secondary_entry
    str   x3, [x2]
    sev                          // Send event to wake WFE
    
    // Wake up CPU2 by writing to its mailbox at 0xE8
    mov   x2, #0xE8
    ldr   x3, =secondary_entry
    str   x3, [x2]
    sev
    
    // Wake up CPU3 by writing to its mailbox at 0xF0
    mov   x2, #0xF0
    ldr   x3, =secondary_entry
    str   x3, [x2]
    sev
    
    b     main

secondary_entry:
    // CPU1/2/3 arrive here after armstub releases them
    mrs   x1, mpidr_el1
    and   x1, x1, #0x3           // Get CPU ID (1, 2, or 3)
    
    // Set up stack for this CPU
    // CPU1: _start + 0x400 + 0x1000 = _start + 0x1400
    // CPU2: _start + 0x400 + 0x2000 = _start + 0x2400
    // CPU3: _start + 0x400 + 0x3000 = _start + 0x3400
    ldr   x2, =_start
    add   x2, x2, #0x400
    mov   x3, #0x1000            // 4KB stack per CPU
    mul   x3, x3, x1             // Multiply by CPU ID
    add   sp, x2, x3             // sp = base + (cpu_id * 4KB)
    
cpu_x:
    wfe
    b     cpu_x