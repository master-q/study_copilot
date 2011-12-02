#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <cwiid.h>
#include "copilot.h"

static cwiid_wiimote_t *g_handle = NULL;

uint32_t g_inited = 0;
uint8_t g_acc0 = 0;
uint8_t g_acc1 = 0;
uint8_t g_acc2 = 0;
uint8_t g_zero0 = 0;
uint8_t g_zero1 = 0;
uint8_t g_zero2 = 0;
uint8_t g_one0 = 0;
uint8_t g_one1 = 0;
uint8_t g_one2 = 0;

// cwiid_wiimote_t *cwiid_open(bdaddr_t *bdaddr, int flags)
void l_cwiid_open(void)
{
	bdaddr_t bdaddr = {0,0,0,0,0,0};

	// if (g_handle == NULL)
	printf("Put Wiimote in discoverable mode now (press 1+2)...\n");
	g_handle = cwiid_open(&bdaddr, 0);
	if (g_handle == NULL) {
		printf("not found...\n");
		exit(1);
	}

	printf("found!\n");
	cwiid_set_led(g_handle, CWIID_LED1_ON | CWIID_LED4_ON);
	g_inited = 1;
}

// cwiid_get_acc_cal(wiimote, CWIID_EXT_NONE, &wm_cal))
void l_cwiid_get_acc_cal(void)
{
	struct acc_cal cal;

	/* xxxx void */ cwiid_get_acc_cal(g_handle, CWIID_EXT_NONE, &cal);
	g_zero0 = cal.zero[0];
	g_zero1 = cal.zero[1];
	g_zero2 = cal.zero[2];
	g_one0 = cal.one[0];
	g_one1 = cal.one[0];
	g_one2 = cal.one[0];

	g_inited = 2;
}

// int cwiid_set_rpt_mode(cwiid_wiimote_t *wiimote, uint8_t rpt_mode);
void l_cwiid_set_rpt_mode(void)
{
	/* xxxx void */ cwiid_set_rpt_mode(g_handle, CWIID_RPT_ACC);
	g_inited = 3;
}

// int cwiid_get_state(cwiid_wiimote_t *wiimote, struct cwiid_state *state)
void l_cwiid_get_state(void) {
	struct cwiid_state st;

	/* xxxx void */ cwiid_get_state(g_handle, &st);
	g_acc0 = st.acc[0];
	g_acc1 = st.acc[1];
	g_acc2 = st.acc[2];
}

double cast_double(uint8_t var) {
	return (double) var;
}

void pout64(uint64_t var)
{
	printf("(%llu)\n", var);
}

void pout_d(double var)
{
	printf("(%f)\n", var);
}

int main()
{
	while (1) {
		step();
		usleep(10*1000);
	}
	return 0; // not reached
}
