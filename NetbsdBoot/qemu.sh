#!/bin/sh

mkdir -p cdrom
cp biosboot/boot cdrom/
cp netbsd_top/sys/arch/i386/stand/bootxx/bootxx_cd9660/bootxx_cd9660 ./

qemu-img create boot.iso 16M
nbmakefs -t cd9660 -o 'bootimage=i386;bootxx_cd9660,no-emul-boot' boot.iso cdrom

qemu-system-i386 -cdrom boot.iso
