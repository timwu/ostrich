#include <ch.h>
#include <hal.h>
#include <avr/interrupt.h>

#include "util.h"

volatile static uint8_t pingState = 0;
volatile static systime_t startTick = 0;

volatile static systime_t distanceTicks = 0;

CH_IRQ_HANDLER(PCINT0_vect) {
  CH_IRQ_PROLOGUE();

  if (pingState == 1) {
    startTick = chTimeNow();
    pingState = 2;
  }
  else if (pingState == 2) {
    distanceTicks = chTimeElapsedSince(startTick);
    pingState = 0;
  }

  CH_IRQ_EPILOGUE();
}

WORKING_AREA(ECHO_THREAD, 64);

static msg_t echoRun(void* ignore) {
  while (true) {
    // Pulse in

    pingState = 1;
    palSetPad(IOPORT1, 0);
    chThdSleepMicroseconds(20);
    palClearPad(IOPORT1, 0);

    printf("Distance ticks is %d\r\n", distanceTicks);

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

  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT0);

  chThdCreateStatic(ECHO_THREAD, sizeof(ECHO_THREAD), NORMALPRIO, echoRun,
      NULL);

  while (true) {
    chThdSleepSeconds(1);
  }
}
