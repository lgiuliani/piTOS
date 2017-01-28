CC = arm-none-eabi-gcc
CFLAGS = -Wall -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -s -g
AS = arm-none-eabi-as
ASFLAGS = -L -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -g

OBJS = startup.o mailbox.o uart.o fb.o entry.o print.o blink.o props.o usb.o

all: $(OBJS)
	arm-none-eabi-ld -T drupelet.ld $(OBJS) -o drupelet.elf
	arm-none-eabi-objcopy -O binary drupelet.elf drupelet.bin

clean:
	rm *.o *.elf *.bin

run:
	qemu-system-arm -M raspi2 -bios drupelet.bin -serial stdio #-dtb ../raspbian/boot/bcm2709-rpi-2-b.dtb
