/*
 * motors.c
 *
 *  Created on: Apr 2, 2014
 *      Author: tim
 */
#include <math.h>
#include <ch.h>
#include <hal.h>
#include "motors.h"
#include "power.h"
#include "util.h"

#define FULL_SPEED 500

typedef struct {
  ioportid_t port;
  uint8_t pad;
  PWMDriver* pwmDriver;
  uint8_t pwmChannel;
} motor_t;

static motor_t FR = { IOPORT5, 4, &PWMD3, 1 };
static motor_t RR = { IOPORT5, 5, &PWMD3, 2 };
static motor_t FL = { IOPORT8, 4, &PWMD4, 1 };
static motor_t RL = { IOPORT8, 3, &PWMD4, 0 };

static PWMConfig pwmConfig = {
    50,
    1023,
    NULL,
    {
      { PWM_OUTPUT_ACTIVE_HIGH, NULL },
      { PWM_OUTPUT_ACTIVE_HIGH, NULL },
      { PWM_OUTPUT_ACTIVE_HIGH, NULL },
    },
};

void motorsSetup() {
  palSetPadMode(FR.port, FR.pad, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(FL.port, FL.pad, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(RR.port, RR.pad, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(RL.port, RL.pad, PAL_MODE_OUTPUT_PUSHPULL);

  pwmStart(&PWMD3, &pwmConfig);
  pwmStart(&PWMD4, &pwmConfig);
}

void motorsSetControl(float pitch, float roll, float yaw, float throttle) {
  if (!isOn()) {
    pwmDisableChannel(FR.pwmDriver, FR.pwmChannel);
    pwmDisableChannel(FL.pwmDriver, FL.pwmChannel);
    pwmDisableChannel(RR.pwmDriver, RR.pwmChannel);
    pwmDisableChannel(RL.pwmDriver, RL.pwmChannel);
    return;
  }

  float frontRightScale = ((pitch + roll + yaw + 3.0f) * (throttle + 1.0f)) / 12.0f;
  float frontLeftScale = ((pitch - roll - yaw + 3.0f) * (throttle + 1.0f)) / 12.0f;
  float rearRightScale = ((-pitch + roll - yaw + 3.0f) * (throttle + 1.0f)) / 12.0f;
  float rearLeftScale = ((-pitch - roll + yaw + 3.0f) * (throttle + 1.0f)) / 12.0f;

  pwmEnableChannel(FR.pwmDriver, FR.pwmChannel, lroundf(frontRightScale * FULL_SPEED));
  pwmEnableChannel(RR.pwmDriver, RR.pwmChannel, lroundf(rearRightScale * FULL_SPEED));
  pwmEnableChannel(FL.pwmDriver, FL.pwmChannel, lroundf(frontLeftScale * FULL_SPEED));
  pwmEnableChannel(RL.pwmDriver, RL.pwmChannel, lroundf(rearLeftScale * FULL_SPEED));
}
