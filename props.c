#include "mailbox.h"
#include "iobase.h"
#include "blink.h"
#include "print.h"
#include "props.h"

volatile struct props8 MB_ALIGN p8 = {
	{sizeof(struct props8), 0},
	{0, 4, 0},
	0,
	0,
	0};

void *props_mail(volatile void *taglist)
{
	unsigned int message = (unsigned int)taglist;
	// message += 0xC0000000;
	mailbox_write(message | 8);
	// TODO the below is required on real hardware. Better install a proper barrier instruction instead
	// wait(4);
	while (MBOX_STATUS(MBOX0_BASE) & MBOX_EMPTY)
		;
	blink(5);
	return (void *)(mailbox_read(8) & ~0xF);
}

unsigned int props8(unsigned int tag, unsigned int selector, unsigned int value)
{
	// set all tags again because a previous call may have overwritten them
	p8.props_header.buffer_size = sizeof(struct props8);
	p8.props_header.req_resp_code = 0;
	p8.prop_tags.id = tag;
	p8.prop_tags.value_size = 8;   // the prop buffer size
	p8.prop_tags.req_resp_ind = 4; // MSB=0=request; remaining 31 bits=prop request size
	p8.selector = selector;
	p8.value = value;
	p8.end_tag = 0;

	props_mail(&p8);

	return p8.value;
}

unsigned int props_4_8(unsigned int tag, unsigned int selector)
{
	return props8(tag, selector, 0);
}
