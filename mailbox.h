#ifndef MAILBOX_H
#define MAILBOX_H

extern void mailbox_write(unsigned int arg_plus_channel);
extern unsigned int mailbox_read(unsigned int channel);

/* Register access helper */
#define REG(x) (*(volatile unsigned int *)(x))

#define MB_ALIGN __attribute__((aligned(16)))

/*
** Mailbox Controller Registers
*/
/* I/O register is at the base address */
#define MBOX_RDWR(x) REG((x))

/* Status register at the offset 0x18 from base */
#define MBOX_STATUS(x) REG((x) + 0x18)

/* Status register value when mailbox is full/empty */
#define MBOX_FULL 0x80000000
#define MBOX_EMPTY 0x40000000

/*  Interrupt configuration register */
#define MBOX_CONFIG(x) REG((x) + 0x1C)

/* Configuration register mask to enable mailbox data IRQ */
#define MBOX_DATAIRQEN 0x00000001

#endif // MAILBOX_H