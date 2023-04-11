TARGET = PiTOS

OUTPUT_IMG = kernel.img

TOOLCHAIN = arm-none-eabi

CC = $(TOOLCHAIN)-gcc
AS = $(TOOLCHAIN)-as
LD = $(TOOLCHAIN)-ld
OBJCOPY =  $(TOOLCHAIN)-objcopy
OBJDUMP = $(TOOLCHAIN)-objdump

CFLAGS = -Wall -mfloat-abi=hard -mfpu=neon-vfpv4 -mfloat-abi=hard -march=armv7-a -mtune=cortex-a7 -s -g
ASFLAGS = -L -mfloat-abi=hard -mfpu=neon-vfpv4 -mcpu=cortex-a7 -g -Wall

OBJS = startup.o mailbox.o uart.o fb.o entry.o print.o blink.o props.o usb.o

all: $(OBJS)
	$(LD) -T linker.ld $(OBJS) -o $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

clean:
	rm *.o *.elf *.bin

run:
	qemu-system-arm  -m 1024 -M raspi2b -bios $(TARGET).bin -serial stdio #-dtb ../raspbian/boot/bcm2709-rpi-2-b.dtb
