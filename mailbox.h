#define MB_ALIGN __attribute__ ((aligned (16)))

extern void mailbox_write(unsigned int arg_plus_channel);
extern unsigned int mailbox_read(unsigned int channel);
