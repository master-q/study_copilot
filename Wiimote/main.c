#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <cwiid.h>
#include "copilot.h"

static cwiid_wiimote_t *g_handle = NULL;

uint16_t g_buttons = 0;
uint8_t g_acc0 = 0; uint8_t g_acc1 = 0; uint8_t g_acc2 = 0;
uint8_t g_zero0 = 0; uint8_t g_zero1 = 0; uint8_t g_zero2 = 0;
uint8_t g_one0 = 0; uint8_t g_one1 = 0; uint8_t g_one2 = 0;

void l_cwiid_open(void)
{
	bdaddr_t bdaddr = {0,0,0,0,0,0};

	printf("Put Wiimote in discoverable mode now (press 1+2)...\n");
	g_handle = cwiid_open(&bdaddr, 0);
	if (g_handle == NULL) {
		printf("not found...\n");
		exit(1);
	}

	printf("found!\n");
	cwiid_set_led(g_handle, CWIID_LED1_ON | CWIID_LED4_ON);
}

void l_cwiid_get_acc_cal(void)
{
	struct acc_cal cal;

	if (cwiid_get_acc_cal(g_handle, CWIID_EXT_NONE, &cal)) {
		printf("*** cwiid_get_acc_cal error");
		exit(2);
	}
	g_zero0 = cal.zero[0]; g_zero1 = cal.zero[1]; g_zero2 = cal.zero[2];
	g_one0 = cal.one[0]; g_one1 = cal.one[0]; g_one2 = cal.one[0];
}

void l_cwiid_set_rpt_mode(void)
{
	if (cwiid_set_rpt_mode(g_handle, CWIID_RPT_ACC | CWIID_RPT_BTN)) {
		printf("*** cwiid_set_rpt_mode error");
		exit(3);
	}
}

void l_cwiid_get_state(void) {
	struct cwiid_state st;

	if (cwiid_get_state(g_handle, &st)) {
		printf("*** cwiid_get_state error");
		exit(4);
	}
	g_buttons = st.buttons;
	g_acc0 = st.acc[0]; g_acc1 = st.acc[1]; g_acc2 = st.acc[2];
}

double cast_double(uint8_t var) {
	return (double) var;
}

void pout_d(double var)
{
	printf("(｀・ω・´) ドギャ! (%f)\n", var);
}

void pout_i(void)
{
	printf("|・ω・｀) イザ...\n");
}

void pout_s(void)
{
	printf("|彡ｻｯ！\n");
}

int main()
{
	while (1) {
		step();
		usleep(10*1000);
	}
	return 0; // not reached
}
