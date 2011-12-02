#include <sys/types.h>
#include <sys/reboot.h>
#include <sys/bootblock.h>

#include <lib/libsa/stand.h>
#include <lib/libsa/ufs.h>
#include <lib/libkern/libkern.h>

#include <libi386.h>
#include "devopen.h"
#include "bootmod.h"

#ifdef SUPPORT_PS2
#include <biosmca.h>
#endif

extern struct x86_boot_params boot_params;
extern	const char bootprog_name[], bootprog_rev[], bootprog_kernrev[];

int boot_biosdev;

void boot2(int, u_int);

void
boot2(int biosdev, u_int biossector)
{
	extern char twiddle_toggle;

	twiddle_toggle = 1;	/* no twiddling until we're ready */
	printf("\f");		/* clear screen (hopefully) */

	initio(boot_params.bp_consdev);

#ifdef SUPPORT_PS2
	biosmca();
#endif
	gateA20();

	if (boot_params.bp_flags & X86_BP_FLAGS_RESET_VIDEO)
		biosvideomode();

	twiddle_toggle = 0;
	printf("\n"
	    ">> %s, Revision %s (from NetBSD %s)\n"
	    ">> for Book:Lambda Ka Musume\n",
	    bootprog_name, bootprog_rev, bootprog_kernrev);
	while (1) {}
}

int
parsebootfile(const char *fname, char **fsname, char **devname,
              int *unit, int *partition, const char **file)
{
	return 0; /* dummy */
}
