#include <mmu.h>
#include <memlayout.h>

    .section .text,"ax",%progbits
    .globl kern_entry
kern_entry:
	csrwi 0x304, 0      # mie
	csrwi 0x344, 0      # mip
    csrwi 0x340, 0      # mscratch
	csrwi 0x180, 0      # satp
	li t0, -1
	csrw 0x302, t0      # medeleg
	csrw 0x303, t0      # mideleg
	csrw 0x306, t0      # mcounteren
	csrw 0x106, t0      # scounteren
    li t0, 1 << 11      # MPP = S
    csrw 0x300, t0      # mstatus
    la t0, kern_init
    csrw 0x341, t0      # mepc
    la sp, bootstacktop
	mret

.section .data
    # .align 2^12
    .align PGSHIFT
    .global bootstack
bootstack:
    .space KSTACKSIZE
    .global bootstacktop
bootstacktop:
