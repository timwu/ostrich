/*
 * motortest.cpp
 *
 *  Created on: Feb 9, 2014
 *      Author: tim
 */

#include <ch.h>
#include <hal.h>

#include "util.h"

static PWMConfig pwmConfig = {
			50,
			1023,
			NULL,
			{
					{PWM_OUTPUT_ACTIVE_HIGH, NULL},
					{PWM_OUTPUT_ACTIVE_HIGH, NULL},
					{PWM_OUTPUT_ACTIVE_HIGH, NULL}
			}
};


int main(void) {
	halInit();
	chSysInit();

	sdStart(&SD1, NULL);
	palSetPadMode((ioportid_t) IOPORT1, 0, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode((ioportid_t) IOPORT2, 6, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode((ioportid_t) IOPORT2, 5, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode((ioportid_t) IOPORT5, 3, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode((ioportid_t) IOPORT5, 4, PAL_MODE_OUTPUT_PUSHPULL);

    printf("Starting up.\n\r");

	pwmStart(&PWMD1, &pwmConfig);
	pwmStart(&PWMD3, &pwmConfig);

	printf("Arming ESCs\n\r");
	pwmEnableChannel(&PWMD1, 1, 40);
	pwmEnableChannel(&PWMD1, 0, 40);
	pwmEnableChannel(&PWMD3, 1, 40);
	pwmEnableChannel(&PWMD3, 0, 40);

	chThdSleepSeconds(1);

	palSetPad(IOPORT1, 0);

	printf("Waiting for ESC to arm.\n\r");
	chThdSleepSeconds(5);

	printf("Spinning up ESC0\n\r");
//	pwmEnableChannel(&PWMD1, 0, 50);
	for (int i = 0; i < 9; i++) {
		pwmEnableChannel(&PWMD1, 1, 40 + (5 * i));
		pwmEnableChannel(&PWMD1, 0, 40 + (5 * i));
		pwmEnableChannel(&PWMD3, 1, 40 + (5 * i));
		pwmEnableChannel(&PWMD3, 0, 40 + (5 * i));
		chThdSleepMilliseconds(200);
	}

	chThdSleepSeconds(3);

	pwmEnableChannel(&PWMD1, 1, 40);
	pwmEnableChannel(&PWMD1, 0, 40);
	pwmEnableChannel(&PWMD3, 1, 40);
	pwmEnableChannel(&PWMD3, 0, 40);

	printf("Stopping test\n\r");

	chThdSleepSeconds(2);

	palClearPad(IOPORT1, 0);

	pwmDisableChannel(&PWMD1, 1);
	pwmDisableChannel(&PWMD1, 0);
	pwmDisableChannel(&PWMD3, 1);
	pwmDisableChannel(&PWMD3, 0);

	while (TRUE) {
		chThdSleepSeconds(2);
	}
}


