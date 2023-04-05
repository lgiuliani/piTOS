CC = arm-none-eabi-gcc
AS = arm-none-eabi-as

CFLAGS = -Wall -mfloat-abi=hard -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -s -g
#CFLAGS = -mfloat-abi=hard  -mfpu=neon-vfpv4 -mcpu=cortex-a7 -ffreestanding -fpic -O2 -lgcc -c -Wall -Wextra -std=gnu99 -nostartfiles
#LDFLAGS = -ffreestanding -O2 -nostdlib -Xlinker --no-warn-rwx-segments
#ASFLAGS = -L -mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -g
ASFLAGS = -L -mfloat-abi=hard -mfpu=neon -mcpu=cortex-a7 -g -Wall
#ASFLAGS = -mcpu=cortex-a7 -ffreestanding -fpic -c

OBJS = startup.o mailbox.o uart.o fb.o entry.o print.o blink.o props.o usb.o

all: $(OBJS)
	arm-none-eabi-ld -T drupelet.ld $(OBJS) -o drupelet.elf
	arm-none-eabi-objcopy -O binary drupelet.elf drupelet.bin

clean:
	rm *.o *.elf *.bin

run:
	qemu-system-arm  -m 1024 -M raspi2b -bios drupelet.bin -serial stdio #-dtb ../raspbian/boot/bcm2709-rpi-2-b.dtb
