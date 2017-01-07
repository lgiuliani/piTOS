#include <stdbool.h>
#include "mailbox.h"
#include "blink.h"

struct {
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
} __attribute__ ((aligned(16))) volatile framebuffer = {
	640, 480, 640, 480, 0, 16, 0, 0, (unsigned short *) 0, 0
};

unsigned short * fb;

unsigned int chars_encoded[] = {
	// special characters
	0x40000000, 0x48421004, 0x14aa0000, 0x00afabea, 0x08ea382e, 0x01111111, 0x1908364d, 0x48440000, 0x04442082, 0x10410888, 0x01157d51, 0x00427c84, 0xc0001088, 0x00003c00, 0x4000018c, 0x00111110,
	// numeric
	0x1d3ae62e, 0x48c2108e, 0x1d11111f, 0x3c11062e, 0x04654be2, 0x3f0f043e, 0x1d0f462e, 0x3e111110, 0x1d17462e, 0x1d18bc2e, 0x40401000, 0x40401100, 0x00444104, 0x000f83e0, 0x00820888, 0x1d111004,
	// uppercase
	0x00eada0f, 0x08a8fe31, 0x3d1f463e, 0x1d18422e, 0x3928c65c, 0x3f0f421f, 0x3f0e4210, 0x1d184e2e, 0x231fc631, 0x5c42108e, 0x0210862e, 0x232e4a31, 0x2108421f, 0x23bac631, 0x239ad671, 0x1d18c62e,
       	0x3d18fa10, 0x1d18d66f, 0x3d18fa51, 0x1d160a2e, 0x3e421084, 0x2318c62e, 0x2318c544, 0x2318d771, 0x22a22a31, 0x23151084, 0x3e22221f, 0x1c84210e, 0x01041041, 0x1c21084e, 0x08a88000, 0x0000001f,
	// lowercase
	0x50820000, 0x00f8c66d, 0x210f463e, 0x00e8c22e, 0x0217c62f, 0x00e8fe0e, 0x1d187210, 0x80f8bc3e, 0x210b6631, 0x48021084, 0xc401084c, 0x21097251, 0x50842106, 0x01bac631, 0x016cc631, 0x00e8c62e,
       	0x81e8fa10, 0x80f8bc21, 0x01e8c210, 0x00e8383e, 0x210e422e, 0x0118c62e, 0x0118c544, 0x0118d771, 0x01151151, 0x8118bc4c, 0x01f1111f, 0x0e426087, 0x48421084, 0x38420c9c, 0x1b600000, 0x2aaaaaaa
};

// NOTE: these vars get into the .bss section, which
// on real hardware is notoriously uninitialized.
// (it's either that or cache plays really weird tricks on them)
// So the real initialization is done in fb_init instead.
unsigned int offsetx = 0;
unsigned int offsety = 0;
bool fixedWidth = true;

unsigned int fb_exchange() {
	unsigned int message = (unsigned int) &framebuffer;
	message += 0xC0000000; // this is where the video chip sees the physical memory
	unsigned int channel = 1;
	wait(4);
	blink(5);
	mailbox_write(message|channel);
	wait(4);
	blink(5);
	return mailbox_read(channel);
}

unsigned int fb_init() {
	unsigned int result = fb_exchange();
	//if(result == 1 && framebuffer.pointer != (unsigned short *)0) fb = framebuffer.pointer;
	if(result == 1 && framebuffer.pointer != (unsigned short *)0) fb = (unsigned short *) (((unsigned int) framebuffer.pointer) - 0xC0000000);

	offsetx = 0;
	offsety=0;
	fixedWidth=true;

	return result;


}

void fb_print(char ch) {
	if (ch < 32) {
		if (ch == '\n') {
			offsetx = 0;
			offsety += 9;
		}

		return;
	}

	ch -= 32;
	unsigned int encoded = chars_encoded[(unsigned int) ch];

	unsigned int width = 5;
	unsigned int height = 6;
	bool stretch = false;

	if ((encoded >> 31) & 1) stretch = true;
	if (((encoded >> 30) & 1) && !fixedWidth) width=3;

	encoded = encoded << 2;

	for (int i=0; i<height; i++) {
		if ((i==2) && stretch) {
			unsigned int line = encoded & (0b11111 << (32-5));
			encoded = (encoded >> 5) | line;
			height += 1;
		}

		if (width == 3) encoded = encoded << 1;
		for (int j=0; j<width;j++) {
			if (encoded & (0x1 << 31)) {
				fb[((i+offsety) * framebuffer.pwidth) + (j+offsetx)] = 0xFFFF;
			}
			encoded = encoded << 1;
		}
		if (width == 3) encoded = encoded << 1;
	}

	offsetx += width+1;
}
