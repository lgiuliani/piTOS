.macro	poll, bit
1:
	mcr	p15, 0, r4, c7, c5, 0	@Flush mem cache so that we get a fresh poll read
	ldr	r2, [r1, #0x18]		@load mailbox status register into r1
	tst	r2, \bit		@check if bit 31 is clear
	bne	1			@if not, poll again
	dmb				@Data Memory Barrier: ensure that acquiring the lock runs before setting the data
	mcr	p15, 0, r4, c7, c5, 0	@To be on the safe side: flush mem cache once more before data is read
.endm

@.section	.text

write_mailbox:	@r0 = message including channel (arg)
	ldr	r1, =0x3F00B880		@load mailbox base address
	poll	#1 << 31
	str	r0, [r1, #0x20]		@ set message (note: r0 is source!)
	dsb				@ Data Synchronization Barrier: perform after write to shared resource
	bx lr

read_mailbox:	@r0 = channel (arg), message including channel (return value)
	ldr	r1, =0x3F00B880		@load mailbox base address
	poll	#1 << 30
	ldr	r2, [r1, #0]		@ get message
	and	r3, r2,#0b1111		@ extract channel
	teq	r3,r0			@ is this the requested channel?
	bne	1			@ if not, try again (jumps to local label '1' of 'poll' macro)
	mov	r0, r2			@ else return value
	bx lr

.global init_fb
init_fb:	@r0 = fb_message pointer (arg), message including channel (return value)
	push	{r4,lr}
	mov	r4,#0			@use r4 as a zero register for the mcr instructions
	@add	r0, r0, #0x40000000	@something with accessing the cached / non-cached version in memory
	orr	r0, #1			@channel
	bl	write_mailbox
	mov	r0,#1			@channel
	bl	read_mailbox
	pop	{r4,pc}
	
