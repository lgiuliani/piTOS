#include <stdbool.h>
#include "mailbox.h"
#include "blink.h"
#include "print.h"
#include "uart.h"
#include "fb.h"
#include "blink.h"

void c_entry() {
	blink(3);

	print("Hello world!\n", uart_print);

	unsigned int init_result = fb_init();

	if (init_result == 1) {
		wait(4);
		blink(4);
	} else {
		wait(4);
		blink(8);
	}

	if (fb != (unsigned short *)0) {
		wait(4);
		blink(2);
	} else {
		wait(4);
		blink(8);
	}

	// simple way to make an interesting looking background
	for (unsigned short i=0; i < framebuffer.pwidth; i++) {
		for (unsigned short j=0; j < framebuffer.pheight; j++) {
			fb[i*j]=i;
		}
	}

	print_pair("\nInit result", init_result);

	print_pair("Framebuffer pointer", (unsigned int) framebuffer.pointer);
	print_pair("pwidth", framebuffer.pwidth);
	print_pair("pheight", framebuffer.pheight);
	print_pair("vwidth", framebuffer.vwidth);
	print_pair("vheight", framebuffer.vheight);
	print_pair("pitch", framebuffer.pitch);
	print_pair("depth", framebuffer.depth);
	print_pair("offsetx", framebuffer.offsetx);
	print_pair("offsety", framebuffer.offsety);
	print_pair("size", framebuffer.size);

	print("\"The quick brown fox jumps over the lazy dog.\"\n", fb_print);
	print("'THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.'\n", fb_print);
	print("0123456789\n", fb_print);
	print("0123456789\n", fb_print);
	print("!@#$%^&*()-_=+[]{}|/\\:;\"'<>,.\n", fb_print);
	fixedWidth = false;
	print("!@#$%^&*()-_=+[]{}|/\\:;\"'<>,.\n", fb_print);
	print("\"The quick brown fox jumps over the lazy dog.\"\n", fb_print);
	print("'THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.'\n", fb_print);

	wait(4);
	blink(6);
}

