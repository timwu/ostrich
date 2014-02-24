#include <ch.h>
#include <hal.h>
#include <avr/interrupt.h>

#include "util.h"

volatile static halrtcnt_t startTick = 0;
volatile static halrtcnt_t distanceTicks = 0;

CH_IRQ_HANDLER(PCINT0_vect) {
  CH_IRQ_PROLOGUE();

  if (palReadPad(IOPORT2, 0) == PAL_HIGH) {
    startTick = halGetCounterValue();
  } else {
    distanceTicks = halGetCounterValue() - startTick;
  }

  CH_IRQ_EPILOGUE();
}

WORKING_AREA(ECHO_THREAD, 64);

static msg_t echoRun(void* ignore) {
  while (true) {
    // Pulse in

    palSetPad(IOPORT1, 0);
    chThdSleepMicroseconds(20);
    palClearPad(IOPORT1, 0);

    printf("Distance ticks is %dus\r\n", distanceTicks);

    chThdSleepMilliseconds(200);

  }
  return (msg_t) 0;
}

int main(void) {
  halInit();
  chSysInit();
  sdStart(&SD1, NULL);

  palSetPadMode((ioportid_t) IOPORT1, 0, PAL_MODE_OUTPUT_PUSHPULL);
  palClearPad(IOPORT1, 0);
  palSetPadMode((ioportid_t) IOPORT2, 0, PAL_MODE_INPUT);

  PCICR |= _BV(PCIE0);
  PCMSK0 |= _BV(PCINT0);

  chThdCreateStatic(ECHO_THREAD, sizeof(ECHO_THREAD), NORMALPRIO, echoRun,
      NULL);

  while (true) {
    chThdSleepSeconds(1);
  }
}
