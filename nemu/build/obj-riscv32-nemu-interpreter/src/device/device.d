cmd_/home/stoair/NJU-PA-2024/nemu/build/obj-riscv32-nemu-interpreter/src/device/device.o := unused

source_/home/stoair/NJU-PA-2024/nemu/build/obj-riscv32-nemu-interpreter/src/device/device.o := src/device/device.c

deps_/home/stoair/NJU-PA-2024/nemu/build/obj-riscv32-nemu-interpreter/src/device/device.o := \
    $(wildcard include/config/target/am.h) \
    $(wildcard include/config/rtc/mmio.h) \
    $(wildcard include/config/has/vga.h) \
    $(wildcard include/config/has/keyboard.h) \
    $(wildcard include/config/i8042/data/mmio.h) \
    $(wildcard include/config/has/serial.h) \
    $(wildcard include/config/has/timer.h) \
    $(wildcard include/config/has/audio.h) \
    $(wildcard include/config/has/disk.h) \
    $(wildcard include/config/has/sdcard.h) \
  /home/stoair/NJU-PA-2024/nemu/include/common.h \
    $(wildcard include/config/mbase.h) \
    $(wildcard include/config/msize.h) \
    $(wildcard include/config/isa64.h) \
  /home/stoair/NJU-PA-2024/nemu/include/macro.h \
  /home/stoair/NJU-PA-2024/nemu/include/debug.h \
  /home/stoair/NJU-PA-2024/nemu/include/utils.h \
    $(wildcard include/config/target/native/elf.h) \
  /home/stoair/NJU-PA-2024/nemu/include/device/alarm.h \

/home/stoair/NJU-PA-2024/nemu/build/obj-riscv32-nemu-interpreter/src/device/device.o: $(deps_/home/stoair/NJU-PA-2024/nemu/build/obj-riscv32-nemu-interpreter/src/device/device.o)

$(deps_/home/stoair/NJU-PA-2024/nemu/build/obj-riscv32-nemu-interpreter/src/device/device.o):
