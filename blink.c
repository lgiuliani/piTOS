#include "print.h"
#include "uart.h"
#include "iobase.h"

// NOTE: almost all numbers in this file are raspi2 specific.
// Not only has io memory moved from 0x20000000 to 0x3f000000,
// but also the OK / ACT LED has moved a gpio pin.

volatile unsigned int * timer_base = (unsigned int *) (IOBASE + 0x00003000);
volatile unsigned int * gpio_base = (unsigned int *) (IOBASE + 0x00200000);

void wait(unsigned int times) {
/*	unsigned int millis = times * 1000;

	volatile unsigned int timer_low = timer_base[4 / 4];
	volatile unsigned int next_timer = timer_base[4 / 4];

	while(next_timer - timer_low < millis) {
		print_num(next_timer, uart_print);
		for (int i=0; i<0x1234;i++) {}
		next_timer = timer_base[4 / 4];
	}*/

	for(int j=0;j<times;j++)
		for(int i=0; i<0x040000; i++) {}
}

void blink_one() {
	wait(1);
	gpio_base[0x20 / 4] |= (1 << 15); // use gpfsel 40 on raspi1
	print("*", uart_print);
	wait(1);
	gpio_base[0x2c / 4] |= (1 << 15); // to turn it off, write to elsewhere...
	print("_", uart_print);
}

void blink(unsigned int times) {

	gpio_base[0x10 / 4] |= (1 << 21);

	for(int i = 0; i < times; i++)
		blink_one();

	print("\n", uart_print);
}
