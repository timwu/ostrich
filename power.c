/*
 * power.c
 *
 *  Created on: Apr 1, 2014
 *      Author: tim
 */

#include <ch.h>
#include <hal.h>
#include "receiver.h"
#include "power.h"

#define POLL_INTERVAL 200

static VirtualTimer vt;
static volatile bool_t on = 0;

static void checkFunc(void * param) {
  if (!receiverGetBoolean(POWER_CHANNEL)) {
    on = TRUE;
    palSetPad(POWER_PORT, POWER_PAD);
  } else {
    on = FALSE;
    palClearPad(POWER_PORT, POWER_PAD);
  }
  chVTSet(&vt, MS2ST(POLL_INTERVAL), checkFunc, NULL);
}

void powerSetup() {
  receiverSetup();
  palSetPadMode(POWER_PORT, POWER_PAD, PAL_MODE_OUTPUT_PUSHPULL);
  palClearPad(POWER_PORT, POWER_PAD);
  chVTSet(&vt, MS2ST(POLL_INTERVAL), checkFunc, NULL);
}

bool_t isOn() {
  return on;
}
