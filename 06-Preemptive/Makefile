CROSS_COMPILE ?= arm-none-eabi-
CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)as
CFLAGS = -fno-common -ffreestanding -O0 \
	 -gdwarf-2 -g3 -Wall -Werror \
	 -DGET_CXT_SWITCH_COST \
	 -mcpu=cortex-m3 -mthumb \
	 -Wl,-Tos.ld -nostartfiles \

OUTDIR = build
TOOLDIR = tool

TARGET = os.bin
SRC = host.c io.c trace.c string.c os.c startup.c \
      context_switch.S syscall.S

export SHELL := /bin/bash

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $^ -o os.elf
	$(CROSS_COMPILE)objcopy -Obinary os.elf $@
	$(CROSS_COMPILE)objdump -S os.elf > os.list

qemu: $(TARGET)
	@qemu-system-arm -M ? | grep stm32-p103 >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo
	qemu-system-arm -M stm32-p103 -semihosting -nographic -kernel $<

qemuauto: $(TARGET)
	bash tool/emulate.sh $<
	cd tool/ && python log2grasp.py && \
		../../../grasp_linux/grasp sched.grasp

qemudbg: $(TARGET)
	qemu-system-arm -M stm32-p103 \
	    -monitor stdio \
	    -gdb tcp::3333 -S \
	    -kernel $(TARGET) -semihosting 2>&1>/dev/null & \
	echo $$! > $(OUTDIR)/qemu_pid && \
	$(CROSS_COMPILE)gdb -x $(TOOLDIR)/gdbscript && \
	cat $(OUTDIR)/qemu_pid | `xargs kill 2>/dev/null || test true` && \
	rm -f $(OUTDIR)/qemu_pid

clean:
	rm -f *.o *.elf *.bin *.list \
		$(TOOLDIR)/*.log $(TOOLDIR)/log $(TOOLDIR)/*.grasp
