# $NetBSD: Makefile.boot,v 1.35 2008/10/19 21:27:46 snj Exp $

S=	${.CURDIR}/../netbsd_top/sys/

NOMAN=
BINDIR= /usr/mdec
BINMODE= 0444
PROG?= boot
NEWVERSWHAT?= "BIOS Boot"
VERSIONFILE?= ${.CURDIR}/../version

SOURCES?= biosboot.S boot2.c
SRCS= ${SOURCES}
.if !make(depend)
SRCS+= vers.c
.endif
SRCS+= copilot.c

PIE_CFLAGS=
PIE_AFLAGS=
PIE_LDFLAGS=

.include <bsd.own.mk>

STRIPFLAG=	# nothing

LIBCRT0=	# nothing
LIBCRTBEGIN=	# nothing
LIBCRTEND=	# nothing
LIBC=		# nothing

BINDIR=/usr/mdec
BINMODE=444

.PATH:	${.CURDIR}/.. $S/arch/i386/stand/lib

LDFLAGS+= -nostdlib -Wl,-N -Wl,-e,boot_start
# CPPFLAGS+= -D__daddr_t=int32_t
CPPFLAGS+= -I ${.CURDIR}/..  -I $S/arch/i386/stand/lib -I ${S}/lib/libsa
CPPFLAGS+= -I ${.OBJDIR}
#CPPFLAGS+= -DDEBUG_MEMSIZE

# Make sure we override any optimization options specified by the user
COPTS=  -Os

.if defined(HAVE_GCC)
.if ${MACHINE} == "amd64"
LDFLAGS+=  -Wl,-m,elf_i386
AFLAGS+=   -m32
CPUFLAGS=  -m32
LIBKERN_ARCH=i386
KERNMISCMAKEFLAGS="LIBKERN_ARCH=i386"
.else
.if ${HAVE_GCC} == 3
CPUFLAGS=  -mcpu=i386
.else
CPUFLAGS=  -march=i386 -mtune=i386
.endif
.endif
.endif

COPTS+=    -ffreestanding
#CFLAGS+= -Wall -Wmissing-prototypes -Wstrict-prototypes
CPPFLAGS+= -nostdinc -D_STANDALONE
CPPFLAGS+= -I$S

CPPFLAGS+= -DSUPPORT_PS2
CPPFLAGS+= -DDIRECT_SERIAL
CPPFLAGS+= -DSUPPORT_SERIAL=boot_params.bp_consdev

CPPFLAGS+= -DCONSPEED=boot_params.bp_conspeed
CPPFLAGS+= -DCONSADDR=boot_params.bp_consaddr
CPPFLAGS+= -DCONSOLE_KEYMAP=boot_params.bp_keymap

CPPFLAGS+= -DSUPPORT_CD9660
CPPFLAGS+= -DSUPPORT_USTARFS
CPPFLAGS+= -DSUPPORT_DOSFS
#CPPFLAGS+= -DSUPPORT_EXT2FS
CPPFLAGS+= -DPASS_BIOSGEOM
CPPFLAGS+= -DPASS_MEMMAP
#CPPFLAGS+= -DBOOTPASSWD
CPPFLAGS+= -DEPIA_HACK

# The biosboot code is linked to 'virtual' address of zero and is
# loaded at physical address 0x10000.
# XXX The heap values should be determined from _end.
SAMISCCPPFLAGS+= -DHEAP_START=0x20000 -DHEAP_LIMIT=0x50000
SAMISCMAKEFLAGS+= SA_USE_CREAD=yes	# Read compressed kernels
SAMISCMAKEFLAGS+= SA_INCLUDE_NET=no	# Netboot via TFTP, NFS

.if (defined(HAVE_GCC) && ${HAVE_GCC} == 4) || defined(HAVE_PCC)
CPPFLAGS+=	-Wno-pointer-sign
.endif

# CPPFLAGS+= -DBOOTXX_RAID1_SUPPORT

I386_STAND_DIR?= $S/arch/i386/stand

CLEANFILES+= machine x86

.if !make(obj) && !make(clean) && !make(cleandir)
.BEGIN:
	-rm -f machine && ln -s $S/arch/i386/include machine
	-rm -f x86 && ln -s $S/arch/x86/include x86
.ifdef LIBOBJ
	-rm -f lib && ln -s ${LIBOBJ}/lib lib
	mkdir -p ${LIBOBJ}/lib
.endif
.endif

### find out what to use for libi386
I386DIR= ${I386_STAND_DIR}/lib
.include "${I386DIR}/Makefile.inc"
LIBI386= ${I386LIB}

### find out what to use for libsa
SA_AS= library
SAMISCMAKEFLAGS+="SA_USE_LOADFILE=yes"
.include "${S}/lib/libsa/Makefile.inc"
LIBSA= ${SALIB}

### find out what to use for libkern
KERN_AS= library
.include "${S}/lib/libkern/Makefile.inc"
LIBKERN= ${KERNLIB}

### find out what to use for libz
Z_AS= library
.include "${S}/lib/libz/Makefile.inc"
LIBZ= ${ZLIB}


cleandir distclean: cleanlibdir

cleanlibdir:
	-rm -rf lib

LIBLIST= ${LIBI386} ${LIBSA} ${LIBZ} ${LIBKERN} ${LIBI386} ${LIBSA}
# LIBLIST= ${LIBSA} ${LIBKERN} ${LIBI386} ${LIBSA} ${LIBZ} ${LIBKERN}

CLEANFILES+= ${PROG}.tmp ${PROG}.map vers.c copilot.c copilot.h

vers.c: ${VERSIONFILE} ${SOURCES} ${LIBLIST} ${.CURDIR}/../Makefile.boot
	${HOST_SH} ${S}conf/newvers_stand.sh -DM ${VERSIONFILE} x86 ${NEWVERSWHAT}

COPILOTFILE?= ${.CURDIR}/../Bootmenu.hs
copilot.c: ${COPILOTFILE}
	runghc ${COPILOTFILE}
	echo >> copilot.h

# Anything that calls 'real_to_prot' must have a %pc < 0x10000.
# We link the program, find the callers (all in libi386), then
# explicitly pull in the required objects before any other library code.
${PROG}: ${OBJS} ${LIBLIST} ${.CURDIR}/../Makefile.boot
	${_MKTARGET_LINK}
	bb="$$( ${CC} -o ${PROG}.tmp ${LDFLAGS} -Wl,-Ttext,0 -Wl,-cref \
	    ${OBJS} ${LIBLIST} | ( \
		while read symbol file; do \
			[ -z "$$file" ] && continue; \
			[ "$$symbol" = real_to_prot ] && break; \
		done; \
		while \
			oifs="$$IFS"; \
			IFS='()'; \
			set -- $$file; \
			IFS="$$oifs"; \
			[ -n "$$2" ] && echo "${I386DST}/$$2"; \
			read file rest && [ -z "$$rest" ]; \
		do :; \
		done; \
	) )"; \
	${CC} -o ${PROG}.tmp ${LDFLAGS} -Wl,-Ttext,0 \
		-Wl,-Map,${PROG}.map -Wl,-cref ${OBJS} $$bb ${LIBLIST}
	${OBJCOPY} -O binary ${PROG}.tmp ${PROG}
#	rm -f ${PROG}.tmp

.include <bsd.prog.mk>
