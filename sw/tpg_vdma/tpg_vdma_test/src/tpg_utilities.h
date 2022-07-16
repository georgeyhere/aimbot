#ifndef TPG_UTILITIES_H
#define TPG_UTILITIES_H

/******************************* Include Files **************************************/
#include "xv_tpg.h"
#include "xvidc.h"
#include <stdio.h>


/******************************** Custom Types **************************************/
typedef struct {
	u32     Height;
	u32     Width;
	u32     ColorFormat;
	u32     MaskId;
	u32     MotionSpeed;
	u32     BackgroundId;
	u32     AutoRestartEnable;

	XV_tpg  Tpg;
} myTpg;

/**************************** Constant Definitions **********************************/

#define TPG_DEVICEID        XPAR_V_TPG_0_DEVICE_ID

/* These constants define the configuration of the TPG on initialization. */
#define TPG_HEIGHT          600
#define TPG_WIDTH           800
#define TPG_COLOR_FORMAT    XVIDC_CSF_RGB
#define TPG_MASK_ID         0x0
#define TPG_MOTION_SPEED    0x4
#define TPG_BGND_ID         XTPG_BKGND_CHECKER_BOARD
#define TPG_AUTO_RESTART    1


/**************************** Function Definitions **********************************/

/**
 * Function to initialize a myTpg instance to configuration parameters defined in
 * tpg_utilities.h
 *
 * @param instancePtr is a pointer to a myTpg instance.
 * @param deviceId is the device ID of the test pattern generator.
 * @return XST_SUCCESSFUL if successful, else XST_FAILURE.
 */
int tpg_initialize(myTpg *instancePtr, uint32_t deviceId);


#endif
