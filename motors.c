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

void motorsSetControl(double pitch, double roll, double yaw, double throttle) {
  if (!isOn()) {
    pwmDisableChannel(FR.pwmDriver, FR.pwmChannel);
    pwmDisableChannel(FL.pwmDriver, FL.pwmChannel);
    pwmDisableChannel(RR.pwmDriver, RR.pwmChannel);
    pwmDisableChannel(RL.pwmDriver, RL.pwmChannel);
    return;
  }

  double frontRightScale = ((-pitch - roll + yaw + 3.0) * (throttle + 1.0)) / 12.0;
  double frontLeftScale = ((-pitch + roll - yaw + 3.0) * (throttle + 1.0)) / 12.0;
  double rearRightScale = ((pitch - roll - yaw + 3.0) * (throttle + 1.0)) / 12.0;
  double rearLeftScale = ((pitch + roll + yaw + 3.0) * (throttle + 1.0)) / 12.0;

  uint16_t frontRightValue = lround(frontRightScale * FULL_SPEED);
  uint16_t frontLeftValue = lround(frontLeftScale * FULL_SPEED);
  uint16_t rearRightValue = lround(rearRightScale * FULL_SPEED);
  uint16_t rearLeftValue = lround(rearLeftScale * FULL_SPEED);

  pwmEnableChannel(FR.pwmDriver, FR.pwmChannel, frontRightValue);
  pwmEnableChannel(RR.pwmDriver, RR.pwmChannel, rearRightValue);
  pwmEnableChannel(FL.pwmDriver, FL.pwmChannel, frontLeftValue);
  pwmEnableChannel(RL.pwmDriver, RL.pwmChannel, rearLeftValue);

//  printf("%f, %f, %f, %f\r\n", frontRightScale, rearRightScale, rearLeftScale, frontLeftScale);
//  printf("%u, %u, %u, %u\r\n", frontRightValue, rearRightValue, rearLeftValue, frontLeftValue);
}
