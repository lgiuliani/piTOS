#include <stdbool.h>
#include "mailbox.h"

volatile unsigned int * const UART0DR = (unsigned int *)0x3F201000;

void uart_print(char s) {
	*UART0DR = (unsigned int)(s); /* Transmit char */
}
