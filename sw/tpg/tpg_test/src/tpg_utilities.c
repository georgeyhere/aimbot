#include "tpg_utilities.h"

/**
 * Function to initialize a myTpg instance to configuration parameters defined in
 * tpg_utilities.h
 *
 * @param instancePtr is a pointer to a myTpg instance.
 * @param deviceId is the device ID of the test pattern generator.
 * @return XST_SUCCESSFUL if successful, else XST_FAILURE.
 */
int tpg_initialize(myTpg *instancePtr, uint32_t deviceId)
{
	int status;

	status = XV_tpg_Initialize(&instancePtr->Tpg, deviceId);
	if(status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	instancePtr->Height            = TPG_HEIGHT;
	instancePtr->Width             = TPG_WIDTH;
	instancePtr->ColorFormat       = TPG_COLOR_FORMAT;
	instancePtr->MaskId            = TPG_MASK_ID;
	instancePtr->MotionSpeed       = TPG_MOTION_SPEED;
	instancePtr->BackgroundId      = TPG_BGND_ID;
	instancePtr->AutoRestartEnable = TPG_AUTO_RESTART;

	XV_tpg_Set_height        (&instancePtr->Tpg, instancePtr->Height);
	XV_tpg_Set_width         (&instancePtr->Tpg, instancePtr->Width);
	XV_tpg_Set_colorFormat   (&instancePtr->Tpg, instancePtr->ColorFormat);
	XV_tpg_Set_maskId        (&instancePtr->Tpg, instancePtr->MaskId);
	XV_tpg_Set_motionSpeed   (&instancePtr->Tpg, instancePtr->MotionSpeed);

	if (instancePtr->AutoRestartEnable) {
		XV_tpg_EnableAutoRestart (&instancePtr->Tpg);
	}

	return XST_SUCCESS;
}


