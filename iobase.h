#ifndef IOBASE_H
#define IOBASE_H

// Should be the right address for BCM2835 / BCM2836
// #define IOBASE 0x7E000000
// But both some forums and at least Qemu say it's this
#define IOBASE 0x3F000000

/* Mailbox 0 base address (Read by ARM) */
#define MBOX0_BASE (IOBASE + 0x0000B880)

/* Mailbox 1 base address (Read by GPU) */
#define MBOX1_BASE (IOBASE + 0x0000B8A0)

#endif // IOBASE_H
