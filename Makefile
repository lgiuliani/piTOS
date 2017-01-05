all:
	arm-none-eabi-as -L -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -g startup.s -o startup.o
	arm-none-eabi-as -L -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -g mailbox.s -o mailbox.o
	arm-none-eabi-gcc -Wall -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -c -s -g uart.c -o uart.o
	arm-none-eabi-gcc -Wall -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -c -s -g fb.c -o fb.o
	arm-none-eabi-gcc -Wall -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -c -s -g entry.c -o entry.o
	arm-none-eabi-gcc -Wall -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -c -s -g print.c -o print.o
	arm-none-eabi-gcc -Wall -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -c -s -g blink.c -o blink.o
	arm-none-eabi-ld -T drupelet.ld startup.o mailbox.o uart.o fb.o entry.o print.o blink.o -o drupelet.elf
	arm-none-eabi-objcopy -O binary drupelet.elf drupelet.bin

clean:
	rm *.o *.elf # *.bin

run:
	qemu-system-arm -M raspi2 -bios drupelet.bin -serial stdio #-dtb ../raspbian/boot/bcm2709-rpi-2-b.dtb
