/*
 * main.c
 *
 *  Created on: Apr 2, 2014
 *      Author: tim
 */
#include <ch.h>
#include <hal.h>
#include "pilot.h"
#include "power.h"
#include "motors.h"
#include "imu.h"
#include "util.h"
#include "pid.h"

#define CLIP(x) ((x) < -1.0 ? -1.0 : (x) > 1.0 ? 1.0 : (x))

static SerialConfig serialConfig = {
    UBRR_F(115200),
    USART_CHAR_SIZE_8
};

static pidcontroller_t pitchPID;
static pidcontroller_t rollPID;
static pidcontroller_t yawPID;

int main(void) {
  halInit();
  chSysInit();

  sdStart(&SD1, &serialConfig);

  pilotSetup();
  powerSetup();
  motorsSetup();
  imuSetup();

  pidInit(&pitchPID, 1.0 / 20000.0, 1.0 / 10000, 0);
  pidInit(&rollPID, 1.0 / 20000.0, 1.0 / 10000, 0);
  pidInit(&yawPID, 1.0 / 20000.0, 1.0 / 10000, 0);

  while (TRUE) {
    printf("yaw rate %f\r\n", imuGetYawRate());
    printf("pitch %f\r\n", imuGetPitch());
    printf("roll %f\r\n", imuGetRoll());
//    double pitch = pilotGetPitch() - pidUpdate(&pitchPID, 0.0, gyroGetPitchRotation());
//    double roll = pilotGetRoll() - pidUpdate(&rollPID, 0, gyroGetRollRotation());
//    double yaw = pilotGetYaw() - pidUpdate(&yawPID, 0, gyroGetYawRotation());
//    motorsSetControl(CLIP(pitch), CLIP(roll), CLIP(yaw), pilotGetThrottle());
//    uint16_t throttle = receiverGetRaw(THROTTLE_CH);
//    uint16_t pitch = receiverGetRaw(PITCH_CH);
//    uint16_t roll = receiverGetRaw(ROLL_CH);
//    uint16_t yaw = receiverGetRaw(YAW_CH);
//    printf("%u, %u, %u, %u\r\n", throttle, pitch, roll, yaw);
//    printf("%u\r\n", receiverGetRaw(4));
//    printf("%f, %f, %f\r\n", receiverGetDouble(PITCH_CH), receiverGetDouble(ROLL_CH), receiverGetDouble(YAW_CH));
//    printf("%d, %d, %d\r\n", gyroGetPitchRotation(), gyroGetRollRotation(), gyroGetYawRotation());
//    printf("%f, %f, %f\r\n", pitch, roll, yaw);
//    printf("%f, %f, %f\r\n", pitchPID.integral, rollPID.integral, yawPID.integral);
    chThdSleepSeconds(1);
  }
}

