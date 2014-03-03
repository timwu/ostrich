#include <ch.h>
#include <hal.h>
#include <avr/interrupt.h>

#include "util.h"
#include "pin_mappings.h"

static halrtcnt_t startTick[6] = {0, 0, 0, 0, 0, 0};
volatile static uint32_t width[6] = {0, 0, 0, 0, 0, 0};

void extInterruptHandler(EXTDriver *extp, expchannel_t channel) {
  if (readIntPad(channel) == PAL_HIGH) {
    startTick[channel] = halGetCounterValue();
  } else {
    width[channel] = halGetCounterValue() - startTick[channel];
  }
}

static EXTConfig extConfig = {
    {
        {EXT_CH_MODE_BOTH_EDGES, extInterruptHandler},
        {EXT_CH_MODE_BOTH_EDGES, extInterruptHandler},
        {EXT_CH_MODE_BOTH_EDGES, extInterruptHandler},
        {EXT_CH_MODE_BOTH_EDGES, extInterruptHandler},
        {EXT_CH_MODE_BOTH_EDGES, extInterruptHandler},
        {EXT_CH_MODE_BOTH_EDGES, extInterruptHandler},
    }
};

int main(void) {
  halInit();
  chSysInit();
  sdStart(&SD1, NULL);
  extStart(&EXTD1, &extConfig);

  palSetPadMode(IOPORT2, 7, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPad(IOPORT2, 7);
  palSetPadMode(IOPORT1, 0, PAL_MODE_OUTPUT_PUSHPULL);
  palClearPad(IOPORT1, 0);

  extChannelEnable(&EXTD1, 0);
  extChannelEnable(&EXTD1, 1);
  extChannelEnable(&EXTD1, 2);
  extChannelEnable(&EXTD1, 3);
  extChannelEnable(&EXTD1, 4);
  extChannelEnable(&EXTD1, 5);
  palSetPadMode(IOPORT2, 0, PAL_MODE_INPUT);

  printf("Starting...\r\n");

  while (true) {
    // Pulse in

    palSetPad(IOPORT1, 0);
    chThdSleepMicroseconds(20);
    palClearPad(IOPORT1, 0);

    printf("Distance ticks are {%u, %u, %u, %u, %u, %u}\r\n", width[0], width[1], width[2], width[3], width[4], width[5]);

    chThdSleepMilliseconds(200);

  }
}
