.macro	poll, bit
1:
	mcr	p15, 0, ip, c7, c5, 0	@ Flush mem cache so that we get a fresh poll read
	ldr	r2, [r1, #0x18]		@ load mailbox status register into r1
	tst	r2, \bit		@ check if bit 31 is clear
	bne	1			@ if not, poll again
	dmb				@ Data Memory Barrier: ensure that acquiring the lock runs before setting the data
	mcr	p15, 0, ip, c7, c10, 5	@ DMB using MCR
	mcr	p15, 0, ip, c7, c5, 0	@ To be on the safe side: flush mem cache once more before data is read
.endm

@.section	.text

.global mailbox_write
mailbox_write:	@r0 = message including channel (arg)
	mov	ip, #0			@ use 'ip' scratch register for the 'mcr' macros
	ldr	r1, =0x3F00B880		@ load mailbox base address
	poll	#1 << 31
	str	r0, [r1, #0x20]		@ set message (note: r0 is source!)
	dsb				@ Data Synchronization Barrier: perform after write to shared resource
	mcr	p15, 0, ip, c7, c10,4	@ DSB using MCR
	bx lr

.global mailbox_read
mailbox_read:	@r0 = channel (arg), message including channel (return value)
	mov	ip, #0			@ use 'ip' scratch register for the 'mcr' macros
	ldr	r1, =0x3F00B880		@ load mailbox base address
	poll	#1 << 30
	ldr	r2, [r1, #0]		@ get message
	and	r3, r2,#0b1111		@ extract channel
	teq	r3,r0			@ is this the requested channel?
	bne	1			@ if not, try again (jumps to local label '1' of 'poll' macro)
	mov	r0, r2			@ else return value
	bx lr
