
#include "util.h"

#include <ch.h>
#include <hal.h>
#include <chprintf.h>


WORKING_AREA(ECHO_THREAD, 64);
WORKING_AREA(PRINTER_THREAD, 128);

static systime_t distanceTicks = 0;

static msg_t echoRun(void* ignore) {
	while (true) {
		// Pulse in
		palSetPad(IOPORT1, 0);
		chThdSleepMicroseconds(20);
		palClearPad(IOPORT1, 0);

		systime_t start = chTimeNow();
		// Wait for leading edge of the echo
		while (palReadPad(IOPORT1, 1) == PAL_LOW) {
			if (chTimeElapsedSince(start) >= MS2ST(100)) {
				goto endloop;
			}
		}

		// Clock in
		start = chTimeNow();

		while (palReadPad(IOPORT1, 1) == PAL_HIGH) {
			if (chTimeElapsedSince(start) >= MS2ST(100)) {
				goto endloop;
			}
		}

		distanceTicks = chTimeElapsedSince(start);

		endloop:
		chThdSleepMilliseconds(1000);

	}
	return (msg_t) 0;
}

static msg_t printerRun(void* ignore) {
	while (true) {
		printf("Distance ticks is %d\r\n", distanceTicks);
		chThdSleepSeconds(1);
	}
	return (msg_t) 0;
}

int main(void) {
	halInit();
	chSysInit();
	sdStart(&SD1, NULL);

	palSetPadMode((ioportid_t) IOPORT1, 0, PAL_MODE_OUTPUT_PUSHPULL);
	palClearPad(IOPORT1, 0);
	palSetPadMode((ioportid_t) IOPORT2, 7, PAL_MODE_OUTPUT_PUSHPULL);
	palClearPad(IOPORT2, 7);
	palSetPadMode((ioportid_t) IOPORT1, 1, PAL_MODE_INPUT);

	chThdCreateStatic(ECHO_THREAD, sizeof(ECHO_THREAD), NORMALPRIO, echoRun, NULL);
	chThdCreateStatic(PRINTER_THREAD, sizeof(PRINTER_THREAD), NORMALPRIO, printerRun, NULL);

	while (true) {
		chThdSleepSeconds(1);
	}
}
