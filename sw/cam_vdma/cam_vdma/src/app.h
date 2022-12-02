#ifndef APP_H
#define APP_H

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "sleep.h"

#include "ov7670.h"
#include "vdma_api.h"

#define XIIC_DEVICE_ID     XPAR_XIICPS_0_DEVICE_ID
#define XIIC_SCLK_RATE     100000

int setup();

#endif

