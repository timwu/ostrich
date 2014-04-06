/*
 * pid.c
 *
 *  Created on: Apr 5, 2014
 *      Author: tim
 */
#include "pid.h"

void pidInit(pidcontroller_t *pid, double p, double i, double d) {
  pid->integral = 0;
  pid->previousError = 0;
  pid->p = p;
  pid->i = i;
  pid->d = d;
  pid->lastTime = halGetCounterValue();
}

double pidUpdate(pidcontroller_t *pid, double setPoint, double measured) {
  double error = setPoint - measured;
  double dt = ((double) (halGetCounterValue() - pid->lastTime)) / halGetCounterFrequency();
  double derivitave = (error - pid->previousError) / dt;

  pid->previousError = error;
  pid->lastTime = halGetCounterValue();
  pid->integral += error * dt;

  return pid->p * error + pid->i * pid->integral + pid->d * derivitave;
}
