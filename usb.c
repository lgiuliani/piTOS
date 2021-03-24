#include "mailbox.h"
#include "blink.h"
#include "print.h"
#include "props.h"
#include "iobase.h"

volatile unsigned int * usb_io = (unsigned int *) (IOBASE + 0x00980000);

const unsigned int OTG_CONTROL = 0x00 / 4;
const unsigned int AHB = 0X08 / 4; // 'advanced microcontroller bus architecture (AMBA) High-performance Bus'
const unsigned int USB = 0X0C / 4;
const unsigned int RESET = 0x10 / 4;
const unsigned int INTERRUPT_MASK = 0x18 / 4;
const unsigned int RECEIVE_SIZE = 0x24 / 4;
const unsigned int NP_FIFO_START_ADDRESS_AND_DEPTH = 0x28 / 4;
const unsigned int HW_DIRECTIONS = 0x44 / 4;
const unsigned int HW_SETTINGS1 = 0x48 / 4;
const unsigned int HW_SETTINGS2 = 0x4C / 4;
const unsigned int HW_SETTINGS3 = 0x50 / 4;
const unsigned int P_FIFO_START_ADDRESS_AND_DEPTH = 0x0100 / 4;
const unsigned int PORT = 0x0440 / 4;
const unsigned int CONFIG = 0x0e00 / 4;
const unsigned int POWER = 0x0e00 / 4;

const unsigned int FIFO_SIZE = 20480;

// TODO Qemu gets stuck here, likely because USB emulation is not properly enabled
// See https://www.raspberrypi.org/forums/viewtopic.php?t=44761&p=355407
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

void hcd_transmit_fifo_flush(int n) {
	usb_io[RESET] = 0;
	usb_io[RESET] |= n << 6; // 5-bit number, where 16= flush all
	usb_io[RESET] |= 1 << 5; // transmit fifo flush
	
	print_pair("Transmit fifo flush", usb_io[RESET]);
	while ((usb_io[RESET] & (1<<5)) != 0) {
	}
	print_pair("Transmit fifo flush done", usb_io[RESET]);
}

void hcd_receive_fifo_flush() {
	usb_io[RESET] = 0;
	usb_io[RESET] |= 1<<4; // receive fifo flush

	print_pair("Receive fifo flush", usb_io[RESET]);
	while ((usb_io[RESET] & (1<<4)) != 0) {
	}
	print_pair("Receive fifo flush done", usb_io[RESET]);
}


/** Basically setting a lot of properties in USB IO memory */
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
void usb_init() {
	// disable interrupts
	usb_io[AHB] &= ~(1 <<0); // Interrupt enable on 'advanced microcontroller bus architecture (AMBA) High-performance Bus'
	usb_io[INTERRUPT_MASK] = 0; // interrupt mask

	print_pair("sizeof int", sizeof(unsigned int));
	print_assert("Turning on USB HCD", props8(SET_POWER_STATE, 3, 1), 1);

	// (end of 'init', begin of 'start')

	usb_io[USB] &= ~((1 << 20) | (1 << 22)); // disable 'ulpi drive external vbus' and 'ts dline pulse enable'
	// TODO is this initial reset required?
	hcd_reset();
	usb_io[USB] &= ~(1 << 3); // disable 'phy interface'
	usb_io[USB] |= (1 << 4); // ModeSelect = UTMI
	hcd_reset();

	print_pair("Hardware settings 1", usb_io[HW_SETTINGS1]);

	// Ensure we have the expected (rpi2 specific) hardware params,
	// so that setting derived params later will be successful

	print_assert("Architecture (should be 2 == internal dma)", (usb_io[HW_SETTINGS1] >> 3) & 0b11, 2);

	print_assert("HighSpeedPhysical (should be 1 == utmi)", (usb_io[HW_SETTINGS1] >> 6) & 0b11, 1);
	print_assert("Ulpifsls", (usb_io[USB] & (1 << 17)) != 0, 0); // = Freescale Stack (?)
	print_assert("ulpi_clk_sus_m", (usb_io[USB] & (1 << 19)) != 0, 0);

	usb_io[AHB] |= (1 << 5) | (1 << 23); // DMA enable / DMA Remainder mode Incremental


	if (print_assert("Operating mode (0 = HNP and SRP capable)", (usb_io[HW_SETTINGS1] >> 0) & 0b111, 0)) {
		usb_io[USB] |= 1 << 8; // set HNP capable
		usb_io[USB] |= 1 << 9; // set SRP capable
		// just checkin'
		print_assert("HNP Capable", (usb_io[USB] & (1 << 8)) != 0, 1);
		print_assert("SRP Capable", (usb_io[USB] & (1 << 9)) != 0, 1);
	}

	usb_io[POWER] = 0; // enable all

	// This assert is a repetition from above
	print_assert("HighSpeedPhysical (should be 1 == utmi)", (usb_io[HW_SETTINGS1] >> 6) & 0b11, 1);
	usb_io[CONFIG] &= ~0b11; // Clock speed = 0b00 = 30 - 60 mhz
	usb_io[CONFIG] |= (1 << 2); // Fsls only = 1

	// FIFO setup
	print_pair("Default FIFO Size", usb_io[RECEIVE_SIZE]);
	print_pair("Default Non-Periodic FIFO host size and depth", usb_io[NP_FIFO_START_ADDRESS_AND_DEPTH]);
	print_pair("Default Periodic FIFO host size and depth", usb_io[P_FIFO_START_ADDRESS_AND_DEPTH]);
	// Defaults should show FIFO size of 1000 and host size of 100.
	// I suggest stick with the defaults unless we understand why we need something else

	usb_io[OTG_CONTROL] |= (1 << 10); // Host set HNP Enable

	// HDC transmit FIFO flush, HCD receive FIFO flush
	hcd_transmit_fifo_flush(16);
	hcd_receive_fifo_flush();
	
	print_pair("Host config enable DMA descriptor", (usb_io[CONFIG] & (1 << 23)));
	if ((usb_io[CONFIG] & (1 << 23)) == 0) {
		// all right, no DMA for you then
		// TODO disable channels
	}
	
	usb_io[PORT] |= (1 << 12); // power = 1
	usb_io[PORT] |= (1 << 8); // reset = true
	// micro delay
	wait(1);
	usb_io[PORT] &= ~(1 << 8); // reset = false again

	print_pair("At end of 'start'", 0); // just log something to say we got here
}
#pragma GCC diagnostic pop

