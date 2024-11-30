.section .data
.global ramdisk_start, ramdisk_end
ramdisk_start:
.incbin "/home/stoair/NJU-PA-2024/nanos-lite/build/ramdisk.img"
ramdisk_end:

.section .rodata
.globl logo
logo:
.incbin "resources/logo.txt"
.byte 0
