#include <ch.h>
#include <hal.h>

static WORKING_AREA(probeWorkingArea, 128);

static msg_t probeThread(void* arg) {
	uint16_t addr = 0;
	while (1) {
		for (addr = 0x00; addr < 0x80; addr++) {
			if (i2cMasterTransmitTimeout(&I2CD1, addr, NULL, 0, NULL, 0, 1000)) {
				chvprintf(&SD1, "Successfully transmitted to addr 0x%x\r\n", &addr);
			}
		}
		chThdSleepMilliseconds(5000);
	}
	return (msg_t) 0;
}

int main(void) {
	halInit();
	chSysInit();

	I2CConfig i2cConfig;
	i2cStart(&I2CD1, &i2cConfig);

	sdStart(&SD1, NULL);

	Thread *t = chThdCreateStatic(probeWorkingArea,
								  sizeof(probeWorkingArea),
								  NORMALPRIO,
								  probeThread,
								  NULL);

	chThdWait(t);
}
