#include "uart.h"
#include "fb.h"

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

        print(s, fb_print);
        print(": ", fb_print);
        print_num(num, fb_print);

}
