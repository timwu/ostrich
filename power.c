/*
 * power.c
 *
 *  Created on: Apr 1, 2014
 *      Author: tim
 */

#include <ch.h>
#include <hal.h>
#include "pilot.h"
#include "power.h"

#define POLL_INTERVAL 200

static VirtualTimer vt;
static volatile bool_t on = 0;

static void checkFunc(void * param) {
  if (!pilotGetPower()) {
    on = true;
    palSetPad(POWER_PORT, POWER_PAD);
  } else {
    on = false;
    palClearPad(POWER_PORT, POWER_PAD);
  }
  chVTSet(&vt, MS2ST(POLL_INTERVAL), checkFunc, NULL);
}

void powerSetup() {
  pilotSetup();
  palSetPadMode(POWER_PORT, POWER_PAD, PAL_MODE_OUTPUT_PUSHPULL);
  palClearPad(POWER_PORT, POWER_PAD);
  chVTSet(&vt, MS2ST(POLL_INTERVAL), checkFunc, NULL);
}

bool_t powerIsOn() {
  return on;
}
