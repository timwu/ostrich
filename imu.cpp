/*
 * gyro.c
 *
 *  Created on: Apr 5, 2014
 *      Author: tim
 */
#include <ch.h>
#include <hal.h>
#include <MPU6050.h>
#include <math.h>
#include "util.h"

#define DISABLE TRUE

#define ALMOST_ZERO 0.25
#define ROUND(x) ((x) < ALMOST_ZERO && (x) > -ALMOST_ZERO ? 0 : (x));
#define CALIBRATION_SAMPLES 128
#define TO_RADS (180 / M_PI)

static int16_t gyroOffset[3];
static double pitchOffset, rollOffset;

static I2CConfig i2cconfig;
static MPU6050 mpu6050;

void imuSetup() {
  i2cStart(&I2CD1, &i2cconfig);
  mpu6050.initialize();

  mpu6050.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  mpu6050.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);

  printf("IMU calibrating...\r\n");

  int32_t gyroSum[3] = {0, 0, 0},
          accelSum[3] = {0, 0, 0};
  int16_t ax, ay, az, gx, gy, gz;
  int16_t i;
  for (i = 0; i < CALIBRATION_SAMPLES; i++) {
    mpu6050.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    accelSum[0] += ax;
    accelSum[1] += ay;
    accelSum[2] += az;
    gyroSum[0] += gx;
    gyroSum[1] += gy;
    gyroSum[2] += gz;
    chThdSleepMilliseconds(1);
  }
  gyroOffset[0] = gyroSum[0] / CALIBRATION_SAMPLES;
  gyroOffset[1] = gyroSum[1] / CALIBRATION_SAMPLES;
  gyroOffset[2] = gyroSum[2] / CALIBRATION_SAMPLES;

  accelSum[0] /= CALIBRATION_SAMPLES;
  accelSum[1] /= CALIBRATION_SAMPLES;
  accelSum[2] /= CALIBRATION_SAMPLES;

  pitchOffset = atan2(accelSum[0], accelSum[2]) * TO_RADS;
  rollOffset = atan2(accelSum[1], accelSum[2]) * TO_RADS;

  printf("Gyro calibration complete. Offsets: (%d, %d, %d)\r\n", gyroOffset[0], gyroOffset[1], gyroOffset[2]);
  printf("Pitch offset = %f, roll offset = %f\r\n", pitchOffset, rollOffset);
}

double imuGetYawRate() {
  return ROUND((mpu6050.getRotationZ() - gyroOffset[2]) / 131.0);
}

double imuGetPitch() {
  int16_t ax, ay, az;
  mpu6050.getAcceleration(&ax, &ay, &az);
  return (atan2(ax, az) * TO_RADS) - pitchOffset;
}

double imuGetRoll() {
  int16_t ax, ay, az;
  mpu6050.getAcceleration(&ax, &ay, &az);
  return (atan2(ay, az) * TO_RADS) - rollOffset;
}
