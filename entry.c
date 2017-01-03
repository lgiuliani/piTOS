#include <stdbool.h>
#include "mailbox.h"
#include "uart.h"
#include "gui.h"

char * hex_range="0123456789ABCDEF";

void print (char * s, void (*print_func) (char s)) {
	int i = 0;
	while(s[i] != 0)
		print_func(s[i++]);
}

void print_num(unsigned int num, void (*print_func) (char s)) {
	        print("0x", print_func);
	        int positions = 7;
	        while (positions >= 0) {
			print_func(hex_range[(num >> (positions * 4)) & 0b1111]);
	                positions--;
	        }
	        print_func('\n');
}

void print_pair(char * s, unsigned int num) {
        print(s, uart_print);
        print(": ", uart_print);
        print_num(num, uart_print);

        print(s, gui_print);
        print(": ", gui_print);
        print_num(num, gui_print);

}

void c_entry() {
	print("Hello world!\n", uart_print);
	unsigned int init_result = init_fb(&framebuffer);
	unsigned short * fb = framebuffer.pointer;

	// simple way to make an interesting looking background
	for (unsigned short i=0; i < framebuffer.pwidth; i++) {
		for (unsigned short j=0; j < framebuffer.pwidth; j++) {
			fb[i*j]=i;
		}
	}

	print_pair("Init result", init_result);
	print_pair("Framebuffer pointer", (unsigned int) fb);
	print_pair("pwidth", framebuffer.pwidth);
	print_pair("pheight", framebuffer.pheight);
	print_pair("vwidth", framebuffer.vwidth);
	print_pair("vheight", framebuffer.vheight);
	print_pair("pitch", framebuffer.pitch);
	print_pair("offsetx", framebuffer.offsetx);
	print_pair("offsety", framebuffer.offsety);
	print_pair("size", framebuffer.size);

	print("\"The quick brown fox jumps over the lazy dog.\"\n", gui_print);
	print("'THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.'\n", gui_print);
	print("0123456789\n", gui_print);
	print("0123456789\n", gui_print);
	print("!@#$%^&*()-_=+[]{}|/\\:;\"'<>,.\n", gui_print);
	fixedWidth = false;
	print("!@#$%^&*()-_=+[]{}|/\\:;\"'<>,.\n", gui_print);
	print("\"The quick brown fox jumps over the lazy dog.\"\n", gui_print);
	print("'THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG.'\n", gui_print);
}

