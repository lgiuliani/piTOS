all:
	arm-none-eabi-as -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -g startup.s -o startup.o
	arm-none-eabi-as -L -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -g mailbox.s -o mailbox.o
	arm-none-eabi-gcc -Wall -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -c -s -g uart.c -o uart.o
	arm-none-eabi-gcc -Wall -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -c -s -g gui.c -o gui.o
	arm-none-eabi-gcc -Wall -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -c -s -g entry.c -o entry.o
	arm-none-eabi-ld -T fb.ld startup.o mailbox.o uart.o gui.o entry.o -o fb.elf
	#arm-none-eabi-objcopy -O binary test.elf test.bin

clean:
	rm *.o *.elf # *.bin

run:
	qemu-system-arm -M raspi2 -kernel fb.elf -serial stdio -dtb ../raspbian/boot/bcm2709-rpi-2-b.dtb
