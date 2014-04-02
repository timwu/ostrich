/*
 * main.c
 *
 *  Created on: Apr 2, 2014
 *      Author: tim
 */
#include <ch.h>
#include <hal.h>
#include "receiver.h"
#include "power.h"

int main(void) {
  halInit();
  chSysInit();
  sdStart(&SD1, NULL);

  receiverSetup();
  powerSetup();

  while (1) {
    chThdSleepSeconds(1);
  }
}

