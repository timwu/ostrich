/*
 * receiver_ppm.c
 *
 *  Created on: Apr 5, 2014
 *      Author: tim
 */
/*
 * receiver.c
 *
 *  Created on: Mar 20, 2014
 *      Author: tim
 */

#include <ch.h>
#include <hal.h>
#include "receiver.h"
#include "pins.h"

#define N_CHANNELS 6
#define WINDOW_SIZE 16
#define SYNC_THRESHOLD 1000

#define ROLLING_AVERAGE FALSE

typedef struct {
  uint16_t min;
  uint16_t max;
  halrtcnt_t start;
#if ROLLING_AVERAGE
  uint8_t slot;
  volatile uint16_t slots[WINDOW_SIZE];
#else
  uint8_t count;
  uint16_t accumulator;
  volatile uint16_t currentValue;
#endif
} channel_t;

// Calibration data
static channel_t channels[N_CHANNELS] = {
    {.min = 280, .max = 490},
    {.min = 295, .max = 456},
    {.min = 293, .max = 461},
    {.min = 279, .max = 475},
    {.min = 475, .max = 475},
    {.min = 475, .max = 475}
};

#if ROLLING_AVERAGE
static uint16_t readChannel(uint8_t channelIndex) {
  uint16_t v = 0;
  uint8_t i = 0;
  channel_t* channel = &channels[channelIndex];
  for (i = 0; i < WINDOW_SIZE; i++) {
    v += channel->slots[i];
  }
  return v / WINDOW_SIZE;
}
#else
#define readChannel(index) (channels[(index)].currentValue)
#endif

static uint8_t currentChannel = 0;
static halrtcnt_t start = 0;

static void isr(EXTDriver* extDriver, expchannel_t channelIndex) {
  uint8_t value = readIntPad(channelIndex);
  channel_t* channel = &channels[currentChannel];
  if (value == PAL_HIGH) {
    start = halGetCounterValue();
  } else {
#if ROLLING_AVERAGE
    channel->slots[channel->slot] = halGetCounterValue() - channel->start;
    channel->slot = (channel->slot + 1) % WINDOW_SIZE;
#else
    halrtcnt_t width = halGetCounterValue() - start;
    if (width > SYNC_THRESHOLD) {
      currentChannel = 0;
    } else {
      channel->accumulator += width;
      if (++channel->count >= WINDOW_SIZE) {
        channel->currentValue = channel->accumulator / channel->count;
        channel->accumulator = 0;
        channel->count = 0;
      }
      currentChannel = (currentChannel + 1) % N_CHANNELS;
    }
  }
#endif
}

static EXTConfig extConfig = {
    {
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT0
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT1
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT2
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT3
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT4
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT5
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT6
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT7
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT8
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT9
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT10
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT11
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT12
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT13
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT14
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT15
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT16
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT17
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT18
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT19
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT20
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT21
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT22
        {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART, isr}, // PCINT23
    }
};

void receiverSetup() {
  extStart(&EXTD1, &extConfig);
}

float receiverGetFloat(uint8_t channel) {
  uint16_t current = readChannel(channel);
  if (current < channels[channel].min) {
    return -1.0;
  } else if (current > channels[channel].max) {
    return 1.0;
  } else {
    return ((current - channels[channel].min) * 2.0f) / (channels[channel].max - channels[channel].min) - 1.0f;
  }
}

uint8_t receiverGetBoolean(uint8_t channel) {
  if (readChannel(channel) > channels[channel].max) {
    return 1;
  } else {
    return 0;
  }
}

uint16_t receiverGetRaw(uint8_t channel) {
  return readChannel(channel);
}

