#include <stdbool.h>
#include "mailbox.h"

extern struct {
	unsigned int pwidth;
	unsigned int pheight;
	unsigned int vwidth;
	unsigned int vheight;
	unsigned int pitch;
	unsigned int depth;
	unsigned int offsetx;
	unsigned int offsety;
	unsigned short * pointer;
	unsigned int size;
} volatile framebuffer;

extern volatile unsigned short * fb;

extern unsigned int fb_init();
extern void fb_print(char ch);

extern bool fixedWidth;
