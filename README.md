Drupelet
========

A prototype kernel for Raspberry Pi 2 that sets up a framebuffer device and
prints characters to it (as well as to UART0).

While these are normal startup requirements for any system, for the Raspberry
Pi they require some setup, including communicating through the GPU mailbox
and providing some kind of character set.

This code is trying to provide these things in a minimalist, simple-to-
understand fashion, as a basis perhaps for more complex projects.


Status
======

The code is known to work on my Raspberry Pi 2 which connects to my
Samsung TV over HDMI. For this, the .bin file resulting from the build
is placed over the existing kernel7.img in the boot partition of a
memory card with a recent Raspbian installation on it.

It is also known to work on the Qemu currently (jan 2017) provided with
Debian Stretch (Qemu 2.7.0 with 'raspi2' machine option); no custom
build of Qemu was required.

The Makefile assumes the presence of the 'arm-none-eabi' version of GCC
and other tools (naming on your system may vary from Debian's).

An initial probing of USB hardware is currently present. It does nothing
too interesting as of yet. Also, it currently hangs on Qemu.


Troubleshooting
===============

No serial output
----------------

You don't get to see any output from UART0 on real hardware unless you install
some kind of serial console, and that's a bit of GPIO tinkering that I haven't
done myself, so YMMV.

On previous versions of Qemu, I have experienced trouble using the _-serial stdio_
option, but it may simply have been that I used the (gp)io base address of
raspi1 (0x20xxxxxxx) instead of raspi2 (0x3fxxxxxx).

(Note: if you want to port this back to raspi1 / zero, be aware that variants
on this base address are currently scattered all over the code. Also see
blink.c , as the IO addresses for the LED have also changed between versions.)


No video output
---------------

It would appear that a resolution of 640x480 is a lowest common denominator.
You can even enforce it through the bootstrapper using hdmi_safe=1 , but I
didn't have to fall back to that myself.

