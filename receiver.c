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

#define MIN 250
#define MAX 510
#define BOOL_THRESHOLD 400

#define ROLLING_AVERAGE FALSE

typedef struct {
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

static channel_t channels[N_CHANNELS];

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

static void isr(EXTDriver* extDriver, expchannel_t channelIndex) {
  uint8_t value = readIntPad(channelIndex);
  channel_t* channel = &channels[channelIndex - 16];
  if (value == PAL_HIGH) {
    channel->start = halGetCounterValue();
  } else {
    halrtcnt_t width = (halGetCounterValue() - channel->start);
    // Filter out bogus signals
    if (width < MIN || width > MAX) {
      return;
    }
#if ROLLING_AVERAGE
    channel->slots[channel->slot] = width;
    channel->slot = (channel->slot + 1) % WINDOW_SIZE;
#else
    channel->accumulator += width;
    if (++channel->count >= WINDOW_SIZE) {
      channel->currentValue = channel->accumulator / channel->count;
      channel->accumulator = 0;
      channel->count = 0;
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
        {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART, isr}, // PCINT16
        {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART, isr}, // PCINT17
        {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART, isr}, // PCINT18
        {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART, isr}, // PCINT19
        {EXT_CH_MODE_BOTH_EDGES | EXT_CH_MODE_AUTOSTART, isr}, // PCINT20
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT21
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT22
        {EXT_CH_MODE_DISABLED, NULL}, // PCINT23
    }
};

void receiverSetup() {
  extStart(&EXTD1, &extConfig);
}

double receiverGetDouble(uint8_t channel) {
  uint16_t current = readChannel(channel);
  return ((current - MIN) * 2.0) / (MAX - MIN) - 1.0;
}

uint8_t receiverGetBoolean(uint8_t channel) {
  return readChannel(channel) > BOOL_THRESHOLD;
}

uint16_t receiverGetRaw(uint8_t channel) {
  return readChannel(channel);
}
