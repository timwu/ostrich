/*
 * pid.h
 *
 *  Created on: Apr 5, 2014
 *      Author: tim
 */

#ifndef PID_H_
#define PID_H_

#include <hal.h>

typedef struct {
  double integral;
  double previousError;
  double p, i, d;
  halrtcnt_t lastTime;
} pidcontroller_t;

#ifdef __cplusplus
extern "C" {
#endif

void pidInit(pidcontroller_t *pid, double p, double i, double d);
double pidUpdate(pidcontroller_t *pid, double setPoint, double measured);

#ifdef __cplusplus
}
#endif

#endif /* PID_H_ */
