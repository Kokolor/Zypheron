.PHONY: all clean disk run

CC := i686-elf-gcc
NASM := nasm
CFLAGS := -g -O2 -Wall -Wextra -ffreestanding -nostdlib -Ikernel -Werror
LDFLAGS := -T kernel/linker.ld -ffreestanding -nostdlib -lgcc
NASMFLAGS := -f elf

SRCS := $(wildcard kernel/*.c)
OBJS := $(patsubst kernel/%.c, build/%.o, $(SRCS)) build/entry.o
KERNEL := build/kernel.elf

all: $(KERNEL)

$(KERNEL): $(OBJS)
	@echo "Linking kernel"
	@mkdir -p $(dir $@)
	@$(CC) $(LDFLAGS) -o $@ $^

build/entry.o: kernel/entry.asm
	@echo "Assembling $<"
	@mkdir -p $(dir $@)
	@$(NASM) $(NASMFLAGS) $< -o $@

build/%.o: kernel/%.c
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -rf build iso_root

disk: $(KERNEL)
	@echo "Creating disk image"
	@mkdir -p build
	@dd if=/dev/zero of=build/kernel.img bs=512 count=93750
	@mkfs.ext2 -F build/kernel.img
	@mkdir -p iso_root/boot/grub
	@cp $(KERNEL) iso_root/boot/
	@cp grub.cfg iso_root/boot/grub/grub.cfg
	@grub-mkrescue -o build/kernel.iso iso_root

run: disk
	@qemu-system-i386 -cdrom build/kernel.iso -debugcon stdio
