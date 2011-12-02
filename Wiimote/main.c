#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <cwiid.h>
#include "copilot.h"

/*
struct cwiid_state {
        uint8_t rpt_mode;
        uint8_t led;
        uint8_t rumble;
        uint8_t battery;
        uint16_t buttons;
        uint8_t acc[3];
        struct cwiid_ir_src ir_src[CWIID_IR_SRC_COUNT];
        enum cwiid_ext_type ext_type;
        union ext_state ext;
        enum cwiid_error error;
};
*/

static cwiid_wiimote_t *g_handle = NULL;

uint32_t g_inited = 0;
uint8_t g_acc0 = 0;
uint8_t g_acc1 = 0;
uint8_t g_acc2 = 0;

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

// int cwiid_set_rpt_mode(cwiid_wiimote_t *wiimote, uint8_t rpt_mode);
void l_cwiid_set_rpt_mode(void)
{
	/* xxxx void */ cwiid_set_rpt_mode(g_handle, CWIID_RPT_ACC);
	g_inited = 2;
}

// int cwiid_get_state(cwiid_wiimote_t *wiimote, struct cwiid_state *state)
void l_cwiid_get_state(void) {
	struct cwiid_state st;

	/* xxxx void */ cwiid_get_state(g_handle, &st);
	g_acc0 = st.acc[0];
	g_acc1 = st.acc[1];
	g_acc2 = st.acc[2];
}

void pout64(uint64_t var)
{
	printf("(%llu)\n", var);
}

int main()
{
	while (1) {
		step();
		usleep(10*1000);
	}
	return 0; // not reached
}
