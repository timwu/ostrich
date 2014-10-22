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

#define MAX_YAW_RATE 90

void pilotSetup() {
  receiverSetup();
}

double pilotGetPitch() {
  return ROUND(receiverGetDouble(PITCH_CH) * 2.0 - 1.0);
}

double pilotGetRoll() {
  return ROUND(receiverGetDouble(ROLL_CH) * 2.0 - 1.0);
}

double pilotGetYawRate() {
  return ROUND(receiverGetDouble(YAW_CH) * 2.0 - 1.0) * MAX_YAW_RATE;
}

double pilotGetThrottle() {
  return receiverGetDouble(THROTTLE_CH);
}

bool pilotGetPower() {
  return receiverGetBoolean(POWER_CH);
}

