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
------

The code is known to work on the Qemu currently (jan 2017) provided with
Debian Stretch (Qemu 2.7.0 with 'raspi2' machine option); no custom
building of Qemu was required.

The Makefile assumes the presence of the 'arm-none-eabi' version of GCC
and other tools (naming on your system may vary from Debian's).

Tests with real hardware have yet to be performed, and I do anticipate
some challenges, at least with handling the 3 other processor cores that
Qemu simply does not provide for.

Not yet present is the initialisation of the USB Controller, which in
turn is required to read the keyboard.
