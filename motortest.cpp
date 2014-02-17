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
	palSetPadMode((ioportid_t) IOPORT2, 7, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode((ioportid_t) IOPORT2, 6, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode((ioportid_t) IOPORT2, 5, PAL_MODE_OUTPUT_PUSHPULL);
	palSetPadMode((ioportid_t) IOPORT2, 4, PAL_MODE_OUTPUT_PUSHPULL);

    printf("Starting up.\n\r");

	pwmStart(&PWMD1, &pwmConfig);
//	pwmStart(&PWMD2, &pwmConfig);

	printf("Arming ESCs\n\r");
//	pwmEnableChannel(&PWMD2, 1, 100);
//	palClearPad(IOPORT1, 0);
	pwmEnableChannel(&PWMD1, 0, 50);
	printf("Used config ICR1=%u OCR1A=%u TCCR1A=0x%x\n\r", ICR1, OCR1A, TCCR1A);
//	pwmEnableChannel(&PWMD1, 2, 50);
//	pwmEnableChannel(&PWMD1, 3, 50);

	chThdSleepSeconds(1);

	palSetPad(IOPORT1, 0);

	printf("Waiting for ESC to arm.\n\r");
	chThdSleepSeconds(5);

	printf("Spinning up ESC0\n\r");
	for (int i = 0; i < 10; i++) {
		pwmEnableChannel(&PWMD1, 0, 50 + i * 50);
		chThdSleepMilliseconds(200);
	}

	chThdSleepSeconds(2);

	printf("Spinning up ESC1\n\r");
	pwmEnableChannel(&PWMD1, 0, 50);
//	pwmEnableChannel(&PWMD1, 1, 500);

//	chThdSleepSeconds(2);

//	pwmEnableChannel(&PWMD1, 1, 50);
//	pwmEnableChannel(&PWMD1, 2, 500);
//
//	chThdSleepSeconds(2);
//
//	pwmEnableChannel(&PWMD1, 2, 50);
//	pwmEnableChannel(&PWMD1, 3, 500);
//
//	chThdSleepSeconds(2);
//
//	pwmEnableChannel(&PWMD1, 3, 50);

	printf("Stopping test\n\r");

	pwmDisableChannel(&PWMD1, 0);
	palClearPad(IOPORT1, 0);

	while (TRUE) {
		chThdSleepSeconds(2);
	}
}


