/*
 * pilot_receiver.c
 *
 *  Created on: Apr 6, 2014
 *      Author: tim
 */
#include "receiver.h"

#define DEAD_ZONE 0.02
#define ROUND(x) ((x) < DEAD_ZONE && (x) > -DEAD_ZONE ? 0.0 : (x))

#define POWER_CH 4
#define PITCH_CH 1
#define ROLL_CH 0
#define YAW_CH 3
#define THROTTLE_CH 2

void pilotSetup() {
  receiverSetup();
}

double pilotGetPitch() {
  return ROUND(receiverGetDouble(PITCH_CH) * 2.0 - 1.0);
}

double pilotGetRoll() {
  return ROUND(receiverGetDouble(ROLL_CH) * 2.0 - 1.0);
}

double pilotGetYaw() {
  return ROUND(receiverGetDouble(YAW_CH) * 2.0 - 1.0);
}

double pilotGetThrottle() {
  return receiverGetDouble(THROTTLE_CH);
}

bool_t pilotGetPower() {
  return receiverGetBoolean(POWER_CH);
}

