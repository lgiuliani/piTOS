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
}

void print_num_dec(unsigned int num, void (* print_func) (char s)) {
	unsigned int divByTen = num / 10;
	if (divByTen != 0) print_num_dec(divByTen, print_func);
	print_func(hex_range[num % 10]);
}

void print_pair_on(char * s, unsigned int num, void (* print_func) (char s)) {
	print(s, print_func);
	print(": ", print_func);
	print_num(num, print_func);
	print("\n", print_func);
}

void print_pair_dec_on(char * s, unsigned int num, void (* print_func) (char s)) {
	print(s, print_func);
	print(": ", print_func);
	print_num_dec(num, print_func);
	print("\n", print_func);
}

void print_pair(char * s, unsigned int num) {
	print_pair_on(s, num, uart_print);
	print_pair_on(s, num, fb_print);
}

void print_pair_dec(char * s, unsigned int num) {
	print_pair_dec_on(s, num, uart_print);
	print_pair_dec_on(s, num, fb_print);
}

bool print_assert_on(char * s, unsigned int num, unsigned int val, void (* print_func) (char s)) {
	print("ERROR: assertion failed: ", print_func);
	print(s, print_func);
	print(". Expected: ", print_func);
	print_num(val, print_func);
	print(", got: ", print_func);
	print_num(num, print_func);
	print("\n", print_func);
}

bool print_assert(char * s, unsigned int num, unsigned int val) {
	if (num != val) {
		print_assert_on(s, num, val, uart_print);
		print_assert_on(s, num, val, fb_print);
		return false;
	} else {
		return true;
	}
}
