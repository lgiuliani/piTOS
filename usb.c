#include "mailbox.h"
#include "blink.h"
#include "print.h"
#include "props.h"

volatile unsigned int * usb_io = (unsigned int *) 0x3F980000;

const unsigned int USB = 0X0C / 4;
const unsigned int RESET = 0x10 / 4;

/*
static struct taglist {
	struct props_header props_header;
	struct prop_header prop_header;
	// value
	unsigned int domain; // 3 = USB HCD
	unsigned int state; // 1 = on
	// end tag
	unsigned int end_tag; // 0 = end
} MB_ALIGN taglist = {
	{sizeof(struct taglist), 0}, {0x00028001, 8, 0}, 3, 1, 0
};*/

void hcd_on() {
	print_pair("USB power-on state", props8(SET_POWER_STATE, 3, 1));
	/*
	unsigned int message = (unsigned int) & taglist;
	//message += 0xC0000000; // shouldn't be necessary here
	print_pair("USB power-on message address", message);
	mailbox_write(message|8); // Enable USB HID chip(?) on mailbox 8 (property interface)
	// TODO the below is required on real hardware. Better install a proper barrier instruction instead
	wait(4);
	blink(5);
	print_pair("As returned (should be the same)", mailbox_read(8));
	print_pair("req_resp_code (should be 0x80000000)", taglist.props_header.req_resp_code);
	print_pair("req_resp_ind (should be 0x80000008)", taglist.prop_header.req_resp_ind); // = MSB for 'ok' + resp length in bytes
	print_pair("state (should be 0x00000001)", taglist.state);
	*/
}

// TODO Qemu gets stuck here (usb and reset are all zeroes); what does Linux do different?
void hcd_reset() {
	print_pair("usb", usb_io[USB]);
	print_pair("reset", usb_io[RESET]);
	while ((usb_io[RESET] & (1 << 31)) == 0) { // 'master idle' should become true
	}
	print_pair("Reset", usb_io[RESET]);

	usb_io[RESET] |= (1 << 0); // soft core reset

	while (((usb_io[RESET] & (1 << 0)) != 0) || ((usb_io[RESET] & (1 << 31)) == 0)) {
		// After a while, master is idle again, and reset is unset again
	}
	print_pair("RESET", usb_io[RESET]);
}

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
void usb_init() {
	// disable interrupts
	usb_io[0x04 / 4] &= ~(1 <<0); // 'advanced microcontroller bus architecture (AMBA) High-performance Bus'
	usb_io[0x18 / 4] = 0; // interrupt mask

	// turn on USB HCD	
	hcd_on();

	// tweak some stuff (don't ask me)
	usb_io[USB] &= ~((1 << 20) | (1 << 22)); // disable 'ulpi drive external vbus' and 'ts dline pulse enable'
	// TODO is this initial reset required?
	//hcd_reset();
	usb_io[USB] &= ~(1 << 3); // disable 'physical interface'
	usb_io[USB] |= (1 << 4); // ModeSelect = UTMI
	hcd_reset();

	print_pair("Hardware (check for high speed phyisical settings)", usb_io[0x44 / 4]);
	print_pair("Usb (match above with these)", usb_io[USB]);
}
#pragma GCC diagnostic pop

