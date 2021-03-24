#include <stdbool.h>
#include "mailbox.h"
#include "iobase.h"

volatile unsigned int * const UART0DR = (unsigned int *) (IOBASE + 0x00201000);

void uart_print(char s) {
	*UART0DR = (unsigned int)(s); /* Transmit char */
}
