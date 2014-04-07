/*
 * gyro.c
 *
 *  Created on: Apr 5, 2014
 *      Author: tim
 */
#include <ch.h>
#include <hal.h>
#include <MPU6050.h>
#include "util.h"

#define DISABLE FALSE

#define ALMOST_ZERO 10
#define ROUND(x) ((x) < ALMOST_ZERO && (x) > -ALMOST_ZERO ? 0 : (x));
#define CALIBRATION_SAMPLES 128

static int16_t pitchOffset = 0;
static int16_t rollOffset = 0;
static int16_t yawOffset = 0;

static I2CConfig i2cconfig;
static MPU6050 mpu6050;

void gyroSetup() {
  i2cStart(&I2CD1, &i2cconfig);
  mpu6050.initialize();

  mpu6050.setFullScaleGyroRange(3);
  printf("Gyro calibrating...\r\n");

  int16_t accumX = 0, accumY = 0, accumZ = 0;
  int16_t i;
  for (i = 0; i < CALIBRATION_SAMPLES; i++) {
    accumX += mpu6050.getRotationX();
    accumY += mpu6050.getRotationY();
    accumZ += mpu6050.getRotationZ();
    chThdSleepMilliseconds(1);
  }
  pitchOffset = accumY / CALIBRATION_SAMPLES;
  rollOffset = accumX / CALIBRATION_SAMPLES;
  yawOffset = accumZ / CALIBRATION_SAMPLES;
  printf("Gyro calibration complete. Offsets: pitch=%d roll=%d yaw=%d\r\n", pitchOffset, rollOffset, yawOffset);
}

int16_t gyroGetPitchRotation() {
#if DISABLE
  return 0;
#else
  return ROUND(mpu6050.getRotationY() - pitchOffset);
#endif
}

int16_t gyroGetRollRotation() {
#if DISABLE
  return 0;
#else
  return ROUND(mpu6050.getRotationX() - rollOffset);
#endif
}

int16_t gyroGetYawRotation() {
#if DISABLE
  return 0;
#else
  return ROUND(mpu6050.getRotationZ() - yawOffset);
#endif
}
